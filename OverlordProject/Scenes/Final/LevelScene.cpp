#include "stdafx.h"
#include "LevelScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Post/PostPixelate.h"


void LevelScene::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.clearColor = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });
	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	// Main Theme
	m_pSystem = SoundManager::Get()->GetSystem();

	//Create sounds
	m_pSystem->createStream("Resources/Sounds/maintrack.mp3", FMOD_DEFAULT, 0, &m_pMainTheme);	
	m_pMainTheme->setMode(FMOD_LOOP_NORMAL);
	
	m_pSystem->createStream("Resources/Sounds/bolts.mp3", FMOD_DEFAULT, 0, &m_pBoltsSound);
	
	//Play maintheme
	m_pSystem->playSound(m_pMainTheme, 0, false, &m_pChannel);
	m_pChannel->setVolume(0.05f);
	m_pChannel->setPaused(true);

	//Simple Level
	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Scene.ovm"));
	auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/color_swatch.png");
	auto triangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Scene.ovpt");
	auto pRigidActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	pRigidActor->AddCollider(PxTriangleMeshGeometry{ triangleMesh }, *pDefaultMaterial);
	pLevelMesh->SetMaterial(pMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;
	characterDesc.actionId_Att = CharacterAtt;

	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pGroundMaterial->SetDiffuseTexture(L"Textures/stars.jpg");

	//Ground Mesh
	//***********
	
	const auto pGroundModel = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(pGroundMaterial);

	const auto pGroundPanel = new GameObject();
	pGroundPanel->AddComponent(pGroundModel);
	pGroundPanel->GetTransform()->Translate(0.0f, 80.0f, 0.0f);
	pGroundPanel->GetTransform()->Scale(100.0f, 100.0f, 100.0f);
	AddChild(pGroundPanel);

	const auto pGroundModel2 = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel2->SetMaterial(pGroundMaterial);

	const auto pLeftPanel = new GameObject();
	pLeftPanel->AddComponent(pGroundModel2);
	pLeftPanel->GetTransform()->Translate(200.0f, 0.f, 0.0f);
	pLeftPanel->GetTransform()->Rotate(0.0f, 0.f, 90.0f);
	pLeftPanel->GetTransform()->Scale(100.0f, 100.0f, 100.0f);
	AddChild(pLeftPanel);

	const auto pGroundModel3 = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel3->SetMaterial(pGroundMaterial);

	const auto pRightPanel = new GameObject();
	pRightPanel->AddComponent(pGroundModel3);
	pRightPanel->GetTransform()->Translate(-200.0f, 0.f, 0.0f);
	pRightPanel->GetTransform()->Rotate(0.0f, 0.f, 90.0f);
	pRightPanel->GetTransform()->Scale(100.0f, 100.0f, 100.0f);
	AddChild(pRightPanel);


	const auto pGroundModel4 = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel4->SetMaterial(pGroundMaterial);

	const auto pForwardPanel = new GameObject();
	pForwardPanel->AddComponent(pGroundModel4);
	pForwardPanel->GetTransform()->Translate(0.0f, 0.f, 200.0f);
	pForwardPanel->GetTransform()->Rotate(90.0f, 0.f, 0.0f);
	pForwardPanel->GetTransform()->Scale(100.0f, 100.0f, 100.0f);
	AddChild(pForwardPanel);

	const auto pGroundModel5 = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel5->SetMaterial(pGroundMaterial);

	const auto pBackPanel = new GameObject();
	pBackPanel->AddComponent(pGroundModel5);
	pBackPanel->GetTransform()->Translate(0.0f, 0.f, -200.0f);
	pBackPanel->GetTransform()->Rotate(90.0f, 0.f, 0.0f);
	pBackPanel->GetTransform()->Scale(100.0f, 100.0f, 100.0f);
	AddChild(pBackPanel);

	m_pCharacter = new Character(characterDesc);
	m_rootCharObject = AddChild(m_pCharacter);
	m_rootCharObject->GetTransform()->Translate(3.f, 5.f, 0.f);
	auto body = new ModelComponent{ L"Meshes/chartest.ovm" };
	const auto charBodyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	charBodyMaterial->SetDiffuseTexture(L"Textures/chartest.png");
	body->SetMaterial(charBodyMaterial);
	GameObject* visuals = new GameObject();
	visuals->AddComponent(body);
	m_pCharacter->AddChild(visuals);
	m_pCharacter->SetVisuals(visuals);
	const float scale = 0.30f;//2.5
	visuals->GetTransform()->Scale(scale, scale, scale);
	visuals->GetTransform()->Translate(0, -1.5, 0);
	visuals->GetTransform()->Rotate(0, 180, 0);

	AddChild(m_pCharacter);
	body->GetAnimator()->SetAnimation(4);
	body->GetAnimator()->SetAnimationSpeed(0.5f);
	body->GetAnimator()->Play();
	m_pCharacter->SetAnimator(body->GetAnimator());

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterAtt, InputState::pressed, 'Q', -1, XINPUT_GAMEPAD_X);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterPause, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);

	m_pPostPixelate = MaterialManager::Get()->CreateMaterial<PostPixelate>();

	AddPostProcessingEffect(m_pPostPixelate);


	m_MenuButtons = new GameObject();
	m_MenuButtons->AddComponent(new SpriteComponent(L"Textures/MenuButtons.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	m_MenuButtons->GetComponent<SpriteComponent>()->GetTransform()->Translate(913, 1210, 0);
	AddChild(m_MenuButtons);

	m_WinScreen = new GameObject();
	m_WinScreen->AddComponent(new SpriteComponent(L"Textures/win.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	m_WinScreen->GetComponent<SpriteComponent>()->GetTransform()->Translate(913, 1210, 0.1f);
	AddChild(m_WinScreen);

	m_HudSprite = new GameObject();
	m_HudSprite->AddComponent(new SpriteComponent(L"Textures/boltshud.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	m_HudSprite->GetComponent<SpriteComponent>()->GetTransform()->Translate(1020, 40, 0.1f);
	m_HudSprite->GetComponent<SpriteComponent>()->GetTransform()->Scale(0.7f, 0.7f, 0.7f);
	AddChild(m_HudSprite);

	InitCrate(8.f, 3.f, 0.f);
	InitCrate(26.f, 3.f, -5.f);
	InitCrate(-16.f, 5.f, -15.f);
	InitCrate(-20.f, 5.f, -10.f);
	InitCrate(-15.f, 5.f, -30.f);
	InitCrate(-18.f, 5.f, 15.f);
	InitCrate(-25.f, 5.f, -0.f);
	InitCrate(0.f, 5.f, -25.f);

	auto destroyCrateParticles = new ParticleEmitterComponent(L"Textures/bolts.png");
	destroyCrateParticles->GetSettings().color = DirectX::XMFLOAT4(0.58f, 0.58f, 0.f, 0.1f);
	destroyCrateParticles->GetSettings().velocity = DirectX::XMFLOAT3(0, 0.f, -0.5);
	destroyCrateParticles->GetSettings().minSize = 1.f;
	destroyCrateParticles->GetSettings().maxSize = 2.f;
	destroyCrateParticles->GetSettings().minEnergy = 0.4f;
	destroyCrateParticles->GetSettings().maxEnergy = 1.2f;
	destroyCrateParticles->GetSettings().minEmitterRadius = 0.1f;
	destroyCrateParticles->GetSettings().maxEmitterRadius = 0.3f;
	m_destoryParticleObj = new GameObject();
	m_destoryParticleObj->AddComponent(destroyCrateParticles);
	m_destoryParticleObj->GetTransform()->Translate(0, 6, 0);
	AddChild(m_destoryParticleObj);
	m_destoryParticleObj->GetComponent<ParticleEmitterComponent>()->SetActive(false);

	m_Text = "0/5";

	m_pControls = new GameObject();
	m_pControls->AddComponent(new SpriteComponent(L"Textures/controls.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Scale(0.5f, 0.5f, 0.5f);
	m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Translate(800, 10000, 0.3f);
	AddChild(m_pControls);
}
void LevelScene::InitCrate(float x, float y, float z)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pCrateObject = AddChild(new GameObject());
	const auto pCrateMesh = pCrateObject->AddComponent(new ModelComponent(L"Meshes/Crate.ovm"));
	auto pCrateMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pCrateMaterial->SetDiffuseTexture(L"Textures/Crate.png");
	auto convexCrateMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Crate.ovpc");
	auto pCrateRigidActor = pCrateObject->AddComponent(new RigidBodyComponent(false));
	pCrateRigidActor->AddCollider(PxConvexMeshGeometry{ convexCrateMesh,PxMeshScale({ 3.f,3.f,3.f }) }, *pDefaultMaterial);
	pCrateMesh->SetMaterial(pCrateMaterial);
	pCrateObject->GetTransform()->Translate(x, y, z);
	pCrateObject->GetTransform()->Scale(3.f, 3.f, 3.f);
	m_Crates.push_back(pCrateObject);
}

void LevelScene::OnGUI()
{
	//m_pCharacter->DrawImGui();
}

void LevelScene::Update()
{ 
	if(m_CollectedBolts >= 5)
	{
		m_Paused = true;
		m_pCharacter->SetPaused(true);
		m_HudSprite->GetComponent<SpriteComponent>()->GetTransform()->Translate(-200, 2000, 0.2f);
		m_WinScreen->GetComponent<SpriteComponent>()->GetTransform()->Translate(-250, 0, 0.2f);
		m_MenuButtons->GetComponent<SpriteComponent>()->GetTransform()->Translate(913, 210, 0);
		m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Translate(800, 10000, 0.3f);
		m_pCharacter->GetTransform()->Translate(-2000, -2000, -2000);
	}
	if (m_SceneContext.pInput->IsActionTriggered(CharacterAtt) && !m_Paused && m_AttAnimTimer == 0.f)
	{
		m_AttAnimTimer = 2.5f;
		for (int i = 0; m_Crates.size() > i; ++i)
		{
			auto currCratePos = m_Crates.at(i)->GetTransform()->GetPosition();
			auto currCharPos = m_rootCharObject->GetTransform()->GetPosition();
			if (5.f > sqrt(((currCratePos.x - currCharPos.x) * (currCratePos.x - currCharPos.x)) + ((currCratePos.y - currCharPos.y) * (currCratePos.y - currCharPos.y)) + ((currCratePos.z - currCharPos.z) * (currCratePos.z - currCharPos.z))))
			{
				m_destoryParticleObj->GetTransform()->Translate(currCratePos.x, currCratePos.y + 1.f, currCratePos.z);
				m_destoryParticleObj->GetComponent<ParticleEmitterComponent>()->SetActive(true);
				m_destoryParticleTimer = 0.8f;
				RemoveChild(m_Crates.at(i), true);
				m_Crates.erase(m_Crates.begin() + i);
				m_pSystem->playSound(m_pBoltsSound, 0, false, &m_pCharacter->GetCharSoundChannel());
				++m_CollectedBolts;
				break;
			}
		}
	}
	
	if(m_SceneContext.pInput->IsActionTriggered(CharacterPause))
	{
		m_Paused = !m_Paused;
		m_pCharacter->SetPaused(m_Paused);
		if(!m_Paused)
		{
			m_pCharacter->UnPauseAnimator();
			m_MenuButtons->GetComponent<SpriteComponent>()->GetTransform()->Translate(913, 1210, 0);
		}
		else
		{
			m_MenuButtons->GetComponent<SpriteComponent>()->GetTransform()->Translate(913, 210, 0);
			m_pCharacter->PauseAnimator();
		}
	}
	if(!m_Paused)
	{
		m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Translate(800, 10000, 0.3f);
		m_pChannel->setPaused(false);
		while (ShowCursor(false) >= 0);
		m_SceneContext.pInput->ForceMouseToCenter(true);
		m_AttAnimTimer -= m_SceneContext.pGameTime->GetElapsed();
		if (m_AttAnimTimer <= 0.f) { m_AttAnimTimer = 0.f; }
		m_destoryParticleTimer -= m_SceneContext.pGameTime->GetElapsed();
		if (m_destoryParticleTimer <= 0.f) { m_destoryParticleTimer = 0.f; }
		if (m_destoryParticleTimer == 0.f)
		{
			m_destoryParticleObj->GetComponent<ParticleEmitterComponent>()->SetActive(false);
		}
	}
	else
	{
		m_pChannel->setPaused(true);
		m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Translate(0, 300	, 0.f);
		if((m_CollectedBolts >= 5))
		{
			m_pControls->GetComponent<SpriteComponent>()->GetTransform()->Translate(800, 10000, 0.f);
		}
		while (ShowCursor(true) <= 0);
		if (m_SceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
		{
			POINT mousePos = InputManager::GetMousePosition();
			if (mousePos.x > 913 && mousePos.y < 580 && mousePos.x < 1300 && mousePos.y > 480)
			{
				PostQuitMessage(WM_QUIT);
			}
			if (mousePos.x > 913 && mousePos.y < 450 && mousePos.x < 1300 && mousePos.y > 350)
			{
				levelRestCount++;
				SceneManager::Get()->AddGameScene(new LevelScene());
				SceneManager::Get()->SetActiveGameScene(L"LevelScene" + std::to_wstring(levelRestCount));
			}

			else if (mousePos.x > 913 && mousePos.y < 320 && mousePos.x < 1300 && mousePos.y > 220)
			{
				levelRestCount++;
				SceneManager::Get()->AddGameScene(new LevelScene());
				SceneManager::Get()->SetActiveGameScene(L"MenuScene");
			}
		}
	}
	if (!(m_CollectedBolts >= 5))
	{
	m_Text = std::to_string(m_CollectedBolts) + " / 5";
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), XMFLOAT2(1080.f, 60.f), XMFLOAT4(0.65f, 0.65f, 0.f, 1.f));
	}
}

void LevelScene::OnSceneActivated()
{
	m_pChannel->setPaused(false);
}

void LevelScene::OnSceneDeactivated()
{
	m_pChannel->setPaused(true);
}