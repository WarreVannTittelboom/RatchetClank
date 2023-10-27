#include "stdafx.h"
#include "SpikyScene.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Materials/DiffuseMaterial.h"


SpikyScene::SpikyScene() :
	GameScene(L"SpikyScene") {}

void SpikyScene::Initialize()
{
	
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	//pMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	//pMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	//pMaterial->SetSpecularTexture(L"Textures/Skulls_Heightmap.tga");
	//pMaterial->SetEnvTexture(L"Textures/Sunol_Cubemap.dds");
	//pMaterial->SetOpacityTexture(L"Textures/Skulls_Heightmap.tga");

	m_pSphere = AddChild(new GameObject());
	auto modelComp = m_pSphere->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"));
	modelComp->SetMaterial(pMaterial);

	m_pSphere->GetTransform()->Translate(0, -5, -15);
	m_pSphere->GetTransform()->Scale(15, 15, 15);


}

void SpikyScene::Update()
{
	m_pSphere->GetTransform()->Rotate(0,20 * m_SceneContext.pGameTime->GetTotal(), 0, true);
}

void SpikyScene::Draw()
{
	//Optional
}

void SpikyScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
	pMaterial->onImGUI();
}
