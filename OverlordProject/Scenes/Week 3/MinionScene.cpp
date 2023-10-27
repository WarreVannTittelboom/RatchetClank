//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "MinionScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Managers/InputManager.h"


MinionScene::MinionScene() :
	GameScene(L"PongScene"){}

void MinionScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;m_SceneContext.settings.enableOnGUI = true;

	//Logger::LogInfo(L"Welcome, humble Minion!");


	const auto pDefaultMaterial = PxGetPhysics().createMaterial(.5f, .5f, 1.0f);
	//GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	
	//Sphere 
	auto pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Red }));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransZ, false);
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	pSphereActor->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	pSphereObject->GetTransform()->Translate(0.f, 0.f, 0.f);
	

	

	//player1
	auto pPlayer1Object = AddChild(new CubePrefab(1.f, 6, 1.f, XMFLOAT4{ Colors::Gray }));
	pPlayer1Actor = pPlayer1Object->AddComponent(new RigidBodyComponent());
	pPlayer1Actor->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransZ, false);
	pPlayer1Actor->SetKinematic(true);
	pPlayer1Actor->AddCollider(PxBoxGeometry(0.5f,3,0.5), *pDefaultMaterial);
	pPlayer1Object->GetTransform()->Translate(-32.f, 0.f, 0.f);

	auto pBoxColliderObject1 = AddChild(new CubePrefab(300, 5, 1, XMFLOAT4{ Colors::Black }));
	pBoxCollider1 = pBoxColliderObject1->AddComponent(new RigidBodyComponent());
	pBoxCollider1->AddCollider(PxBoxGeometry(150, 2.5f, 0.5f),*pDefaultMaterial);
	pBoxCollider1->SetKinematic(true);
	pBoxColliderObject1->GetTransform()->Translate(0, 23, 0);


	auto pBoxColliderObject2 = AddChild(new CubePrefab(300, 5, 1, XMFLOAT4{ Colors::Black }));
	pBoxCollider2 = pBoxColliderObject2->AddComponent(new RigidBodyComponent());
	pBoxCollider2->AddCollider(PxBoxGeometry(150, 2.5f, 0.5f), *pDefaultMaterial);
	pBoxCollider2->SetKinematic(true);
	pBoxColliderObject2->GetTransform()->Translate(0, -23, 0);
	//player2
	auto pPlayer2Object = AddChild(new CubePrefab(1.f, 6, 1.f, XMFLOAT4{ Colors::Gray }));
	pPlayer2Actor = pPlayer2Object->AddComponent(new RigidBodyComponent());
	pPlayer2Actor->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransZ, false);
	pPlayer2Actor->SetKinematic(true);
	pPlayer2Actor->AddCollider(PxBoxGeometry(0.5f, 3, 0.5), *pDefaultMaterial);
	pPlayer2Object->GetTransform()->Translate(32.f, 0.f, 0.f);


	//background
	auto pBackGroundObject = AddChild(new CubePrefab(300.f, 300, 1.f, XMFLOAT4{ Colors::Black }));
	pBackGround = pBackGroundObject->AddComponent(new RigidBodyComponent());
	pBackGround->SetConstraint(RigidBodyConstraint::AllRot | RigidBodyConstraint::TransZ, false);
	pBackGround->SetKinematic(true);
	pBackGround->AddCollider(PxBoxGeometry(0.5f, 3, 0.5), *pDefaultMaterial);
	pBackGroundObject->GetTransform()->Translate(0.f, 0.f, 60.f);

	pSphereObject->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER && pOther == pPlayer1Object)
			{
				std::cout << "test1";
				std::cout << pTrigger;
				pSphereActor->Translate(XMFLOAT3{ 0.f, 0.f, 0.f });
			}

			if (action == PxTriggerAction::ENTER && pOther == pPlayer2Object)
			{
				std::cout << "test2";
				std::cout << pTrigger;
				pSphereActor->Translate(XMFLOAT3{ 0.f, 0.f, 0.f });
			}
			
		});
}

void MinionScene::Update()
{
	if(InputManager::IsKeyboardKey(InputState::down, 'Q'))
	{
		pPlayer1Actor->Translate(XMFLOAT3{ pPlayer1Actor->GetTransform()->GetPosition().x, pPlayer1Actor->GetTransform()->GetPosition().y + 0.1f , pPlayer1Actor->GetTransform()->GetPosition().z });
	}

	if (InputManager::IsKeyboardKey(InputState::down, 'A'))
	{
		pPlayer1Actor->Translate(XMFLOAT3{ pPlayer1Actor->GetTransform()->GetPosition().x, pPlayer1Actor->GetTransform()->GetPosition().y - 0.1f , pPlayer1Actor->GetTransform()->GetPosition().z });
	}

	if (InputManager::IsKeyboardKey(InputState::down, VK_UP))
	{
		pPlayer2Actor->Translate(XMFLOAT3{ pPlayer2Actor->GetTransform()->GetPosition().x, pPlayer2Actor->GetTransform()->GetPosition().y + 0.1f , pPlayer2Actor->GetTransform()->GetPosition().z });
	}

	if (InputManager::IsKeyboardKey(InputState::down, VK_DOWN))
	{
		pPlayer2Actor->Translate(XMFLOAT3{ pPlayer2Actor->GetTransform()->GetPosition().x, pPlayer2Actor->GetTransform()->GetPosition().y - 0.1f , pPlayer2Actor->GetTransform()->GetPosition().z });
	}
	if (InputManager::IsKeyboardKey(InputState::down, 'E'))
	{
		pSphereActor->Translate(XMFLOAT3{ 0.f, 0.f, 0.f });
		pSphereActor->AddForce(XMFLOAT3{ 2500,1250,0 }, PxForceMode::eFORCE);
	}

	if(pSphereActor->GetTransform()->GetPosition().x > 40.0f || pSphereActor->GetTransform()->GetPosition().x < -40.f)
	{
		pSphereActor->Translate(XMFLOAT3{ 0.f, 0.f, 0.f });
	}	
	
}

void MinionScene::Draw()
{
	//Optional
}

void MinionScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
