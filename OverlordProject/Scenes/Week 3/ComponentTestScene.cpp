//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Materials/DiffuseMaterial.h"


ComponentTestScene::ComponentTestScene() :
	GameScene(L"ComponentTestScene"){}

void ComponentTestScene::Initialize()
{
	////m_SceneContext.settings.showInfoOverlay = true;
	////m_SceneContext.settings.drawPhysXDebug = true;
	////m_SceneContext.settings.drawGrid = trues;
	//m_SceneContext.settings.enableOnGUI = true;

	//const auto pDefaultMaterial = PxGetPhysics().createMaterial(.5f, .5f, 1.0f);
	//GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	////Sphere 1
	//auto pSphereObject = AddChild(new SpherePrefab(1.f,10,XMFLOAT4{Colors::Red}));
	//auto pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	//pSphereActor->AddCollider(PxSphereGeometry(1.f),*pDefaultMaterial);
	//pSphereObject->GetTransform()->Translate(0.f, 55.f, 0.f);

	//pSphereActor->SetCollisionGroup(CollisionGroup::Group1);
	//pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group2);

	////Sphere 2
	//pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Green }));
	//pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	//pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	//pSphereObject->GetTransform()->Translate(0.f, -55.f, 0.f);

	//pSphereActor->SetCollisionGroup(CollisionGroup::Group2);
	//pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1);

	////Sphere 3
	//pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Blue }));
	//pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	//pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	//pSphereObject->GetTransform()->Translate(0.f, 30.f, 0.f);

	////pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1);

	////ColorMaterial* pMaterial = new ColorMaterial();

	//m_pChair->AddComponent(new ModelComponent(L"Meshes/Chair.ovm"));
	//m_pChair->GetTransform()->Translate(0.f, 20.f, 0.f);
	////m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	///
	////m_SceneContext.settings.showInfoOverlay = true;
	///
  /*  m_SceneContext.settings.drawPhysXDebug = false;
    m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/color_swatch.png");
	m_pChair = AddChild(new GameObject());

	auto modelComp = m_pChair->AddComponent(new ModelComponent(L"Meshes/Scene.ovm"));
	

	auto triangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Scene.ovpt");

	auto pSphereActor = m_pChair->AddComponent(new RigidBodyComponent(true));
	pSphereActor->AddCollider(PxTriangleMeshGeometry{ triangleMesh }, *pDefaultMaterial);*/

	
	
}

void ComponentTestScene::Update()
{
	
	//Optional
}

void ComponentTestScene::Draw()
{
	//Optional
}

void ComponentTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
