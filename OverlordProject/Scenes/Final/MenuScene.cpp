#include "stdafx.h"
#include "MenuScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Scenes/Final/LevelScene.h"

void MenuScene::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawGrid = false;

	GameObject* backGround = new GameObject();
	backGround->AddComponent(new SpriteComponent(L"Textures/MainMenu.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	backGround->GetTransform()->Translate(0, 0, 0.5f);
	AddChild(backGround);
	GameObject* controls = new GameObject();
	controls->AddComponent(new SpriteComponent(L"Textures/controls.png", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)));
	controls->GetTransform()->Scale(0.4f, 0.4f, 0.4f);
	controls->GetTransform()->Translate(800, 320, 0.1f);
	AddChild(controls);
	ShowCursor(true);
}

void MenuScene::Update()
{
	if (m_SceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
	{
		POINT mousePos = InputManager::GetMousePosition();
		if(mousePos.x > 50 && mousePos.y < 400 && mousePos.x < 420 && mousePos.y > 300)
		{
			PostQuitMessage(WM_QUIT);
		}

		else if (mousePos.x > 50 && mousePos.y < 250 && mousePos.x < 420 && mousePos.y > 120)
		{
			SceneManager::Get()->SetActiveGameScene(L"LevelScene" + std::to_wstring(LevelScene::levelRestCount));
		}
	}
}

void MenuScene::OnGUI()
{
	
}
