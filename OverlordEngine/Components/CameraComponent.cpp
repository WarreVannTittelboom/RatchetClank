#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{

	const auto mousePos = m_pScene->GetSceneContext().pInput->GetMousePosition();
	const auto halfwidth = m_pScene->GetSceneContext().windowWidth / 2;
	const auto halfheigt = m_pScene->GetSceneContext().windowHeight / 2;


	float xndc = (mousePos.x - halfwidth) / halfwidth;
	float yndc = (halfheigt - mousePos.y) / halfheigt;

	XMMATRIX viewInverse = XMLoadFloat4x4(&m_ViewProjectionInverse);
	XMVECTOR nearPoint = XMVector3TransformCoord({ xndc, yndc, 0, 0 }, viewInverse);
	XMVECTOR farPoint = XMVector3TransformCoord({ xndc, yndc, 1, 0 }, viewInverse);
	XMFLOAT3 nearpoint;
	XMStoreFloat3(&nearpoint, nearPoint);
	XMFLOAT3 farpoint;
	XMStoreFloat3(&farpoint, farPoint); 
	PxQueryFilterData filterdata{};
	filterdata.data.word0 = ~UINT(ignoreGroups);
	PxRaycastBuffer hit{};
	PxVec3 rayOrigin = PxVec3(nearpoint.x, nearpoint.y, nearpoint.z);
	PxVec3 rayDir = PxVec3(farpoint.x - nearpoint.x, farpoint.y - nearpoint.y, farpoint.z - nearpoint.z);
	rayDir.normalize();
	if(m_pScene->GetPhysxProxy()->Raycast(rayOrigin,rayDir,PX_MAX_F32,hit,PxHitFlag::eDEFAULT,filterdata))
	{
		auto actor = hit.getAnyHit(0).actor;

		if(actor)
		{
			auto userData = actor->userData;
			if(userData)
			{
				BaseComponent* obj = reinterpret_cast<BaseComponent*>(userData);
				return obj->GetGameObject();
			}
		}
	} 

	return nullptr;
}