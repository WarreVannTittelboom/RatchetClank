#pragma once
class PostPixelate;
class Character;

class LevelScene : public GameScene
{
public:
	static inline int levelRestCount = 0;
	LevelScene() :GameScene(L"LevelScene" + std::to_wstring(levelRestCount)) {}
	~LevelScene() override = default;
	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) noexcept = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) noexcept = delete;

	void InitCrate(float x, float y, float z);

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterAtt,
		CharacterPause
	};

	Character* m_pCharacter{};
	GameObject* m_rootCharObject{};
	PostPixelate* m_pPostPixelate = nullptr;
	bool m_Paused = false;
	GameObject* m_MenuButtons = nullptr;
	GameObject* m_WinScreen = nullptr;
	GameObject* m_HudSprite = nullptr;
	std::vector <GameObject*> m_Crates;
	float m_AttAnimTimer{};
	GameObject* m_destoryParticleObj{};
	float m_destoryParticleTimer{};
	SpriteFont* m_pFont{};
	int m_CollectedBolts = 0;
	GameObject* m_pControls{};
	std::string m_Text{};
	FMOD::Sound* m_pMainTheme{};
	FMOD::Sound* m_pAttSound{};
	FMOD::Sound* m_pBoltsSound{};
	FMOD::Channel* m_pChannel{};
	FMOD::System* m_pSystem{};
};

