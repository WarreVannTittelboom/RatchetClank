#pragma once
struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
		//controller.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
	}

	float maxMoveSpeed{ 26.f };
	float maxFallSpeed{ 10.f };

	float JumpSpeed{ 25.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };
	float minPitch{ -15.f };
	float maxPitch{ 30.f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 35.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
	int actionId_Att{ -1 };
};

class Character : public GameObject
{
public:
	Character(const CharacterDesc& characterDesc);
	~Character() override = default;

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	//bool GetIsOnGround() const;

	void DrawImGui();

	void SetAnimator(ModelAnimator* pAnimator);
	void PauseAnimator();
	void UnPauseAnimator();
	void SetVisuals(GameObject* visuals) { m_Visuals = visuals; }
	void SetPaused(bool paused) { m_Paused = paused; }
	FMOD::Channel *& GetCharSoundChannel() { return m_pChannel; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)
	GameObject* m_pCameraBoom{};
	ModelAnimator* m_pAnimator{};
	GameObject* m_Visuals{};
	float m_AttAnimTimer{};
	bool m_Paused{ false };
	FMOD::Sound* m_pAttSound{};
	FMOD::Sound* m_pJumpSound{};
	FMOD::Channel* m_pChannel{};
	FMOD::System* m_pSystem{};
};

