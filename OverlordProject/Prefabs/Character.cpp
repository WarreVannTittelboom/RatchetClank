#include "stdafx.h"
#include "Character.h"
#include <memory>

Character::Character(const CharacterDesc& characterDesc) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
{}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	m_pSystem = SoundManager::Get()->GetSystem();

	m_pSystem->createStream("Resources/Sounds/att.mp3", FMOD_DEFAULT, 0, &m_pAttSound);
	m_pSystem->createStream("Resources/Sounds/jump.mp3", FMOD_DEFAULT, 0, &m_pJumpSound);
	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();


	GameObject* go = new GameObject();
	go->AddComponent(m_pCameraComponent);
	m_pCameraBoom = new GameObject;
	m_pCameraBoom->AddChild(go);
	AddChild(m_pCameraBoom);
	go->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, -10.f);

	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	//pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, -10.f);

}

void Character::Update(const SceneContext& /*sceneContext*/)
{
	if (m_pCameraComponent->IsActive() && !m_Paused)
	{
		auto pInput = GetScene()->GetSceneContext().pInput;
		auto d_time = GetScene()->GetSceneContext().pGameTime->GetElapsed();

		m_AttAnimTimer -= d_time;
		if (m_AttAnimTimer <= 0.f) { m_AttAnimTimer = 0.f; }
		//constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero

		XMFLOAT2 move = XMFLOAT2(); //Uncomment

		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		{
			move.y += 1;

		}
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		{
			move.y -= 1;

		}

		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
		//if (abs(move.y) < epsilon)
			//move.y = InputManager::GetThumbstickPosition().y;

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		{
			move.x += 1;

		}
		if (pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		{
			move.x -= 1;

		}

		if (move.x == 0 && move.y == 0)
		{
			move = InputManager::GetThumbstickPosition(true);
		}

		XMFLOAT2 look{ 0.f, 0.f }; //Uncomment

			// Store the MouseMovement in the local 'look' variable (cast is required)
		look.x = float(InputManager::GetMouseMovement().x);
		look.y = float(InputManager::GetMouseMovement().y);

		if (look.x == 0 && look.y == 0)
		{
			look = InputManager::GetThumbstickPosition(false);
			look.x *= m_CharacterDesc.rotationSpeed * d_time * 8;
			look.y *= -m_CharacterDesc.rotationSpeed * d_time * 8;
		}

		//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
		//if (abs(look.x) < epsilon && abs(look.y) < epsilon)
			//look = InputManager::GetThumbstickPosition(false);


		//************************
		//GATHERING TRANSFORM INFO

		//Retrieve the TransformComponent
		auto pTrans = m_pCameraBoom->GetTransform();
		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		XMVECTOR forward = XMLoadFloat3(&pTrans->GetForward());
		XMVECTOR right = XMLoadFloat3(&pTrans->GetRight());
		bool isMoving = true;

		//***************
		//CAMERA ROTATION

		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
		//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
		m_TotalYaw += look.y * d_time * m_CharacterDesc.rotationSpeed;
		m_TotalPitch += look.x * d_time * m_CharacterDesc.rotationSpeed;

		if (m_TotalYaw < m_CharacterDesc.minPitch)
		{
			m_TotalYaw = m_CharacterDesc.minPitch;
		}
		if (m_TotalYaw > m_CharacterDesc.maxPitch)
		{
			m_TotalYaw = m_CharacterDesc.maxPitch;
		}

		//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)
		pTrans->Rotate(m_TotalYaw, m_TotalPitch, 0);

		//********
		//MOVEMENT

		//## Horizontal Velocity (Forward/Backward/Right/Left)
		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
		auto currentMoveAcc = m_MoveAcceleration * d_time;
		//If the character is moving (= input is pressed)
		if (move.x != 0 || move.y != 0)
		{
			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
			m_CurrentDirection = { 0,0,0 };
			auto currPos = XMLoadFloat3(&m_CurrentDirection);
			currPos += forward * move.y * m_MoveSpeed * d_time;
			currPos += right * move.x * m_MoveSpeed * d_time;

			XMStoreFloat3(&m_CurrentDirection, currPos);

			//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed += currentMoveAcc;
			//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
				m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
		}
		//Else (character is not moving, or stopped moving)
		else
		{
			//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed -= currentMoveAcc;
			//Make sure the current MoveSpeed doesn't get smaller than zero
			if (m_MoveSpeed < 0)
				m_MoveSpeed = 0;

			m_TotalVelocity.x = 0;
			m_TotalVelocity.z = 0;
			isMoving = false;
		}

		//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
		auto horizontalVelocity = XMLoadFloat3(&m_CurrentDirection) * m_MoveSpeed;
		XMFLOAT3 l{};
		XMStoreFloat3(&l, horizontalVelocity);
		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
		m_TotalVelocity.x = l.x;
		m_TotalVelocity.z = l.z;
		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)

		//## Vertical Movement (Jump/Fall)
		//If the Controller Component is NOT grounded (= freefall)
		if (!(m_pControllerComponent->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)))
		{
			//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
			m_TotalVelocity.y -= m_FallAcceleration * d_time;
			//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
			if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
				m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
		}
		//Else If the jump action is triggered

		else if (pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
		{
			if (m_pAnimator && m_pAnimator->GetClipNumber() != 1 && m_AttAnimTimer == 0.f)
			{
				m_pSystem->playSound(m_pJumpSound, 0, false, &m_pChannel);
				// if not already playing this clip
				m_pAnimator->SetAnimation(1);
				m_pAnimator->SetAnimationSpeed(0.6f);
				m_pAnimator->Play();
				m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
			}
		}
		//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed

	//Else (=Character is grounded, no input pressed)
		else
		{
			if (m_AttAnimTimer == 0.f)
			{
				if (pInput->IsActionTriggered(m_CharacterDesc.actionId_Att))
				{
					m_pSystem->playSound(m_pAttSound, 0, false, &m_pChannel);
					if (m_pAnimator && m_pAnimator->GetClipNumber() != 2)
					{
						// if not already playing this clip
						m_pAnimator->SetAnimation(2);
						m_pAnimator->SetAnimationSpeed(0.7f);
						m_pAnimator->Play();
						m_AttAnimTimer = 2.5f;
					}
				}
				else if (isMoving)
				{
					// play walk/run/run_hard animation
					if (m_pAnimator && m_pAnimator->GetClipNumber() != 0)
					{
						// if not already playing this clip
						m_pAnimator->SetAnimation(0);
						m_pAnimator->SetAnimationSpeed(0.6f);
						m_pAnimator->Play();
					}
				}
				else
				{
					if (m_pAnimator && m_pAnimator->GetClipNumber() != 4)
					{
						// if not already playing this clip
						m_pAnimator->SetAnimation(4);
						m_pAnimator->SetAnimationSpeed(0.5f);
						m_pAnimator->Play();
					}
				}
			}
			m_TotalVelocity.y = 0;
		}
		//m_TotalVelocity.y is zero




		if (m_TotalVelocity.x != 0 || m_TotalVelocity.z != 0)
		{
			const float pi = 3.1415926535f;
			float angle = atan2(GetTransform()->GetForward().z, GetTransform()->GetForward().x) - atan2(m_TotalVelocity.z, m_TotalVelocity.x);
			angle *= (180 / pi);
			m_Visuals->GetTransform()->Rotate(0, angle + 180, 0);
		}
		//************
		//DISPLACEMENT
		//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
		auto vel = XMLoadFloat3(&m_TotalVelocity);
		vel *= d_time;
		XMFLOAT3 displacement{};
		XMStoreFloat3(&displacement, vel);
		//Calculate the displacement (m) for the current frame and move the ControllerComponent
		m_pControllerComponent->Move(displacement);

		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
		//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
	}
}

void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}

void Character::SetAnimator(ModelAnimator* pAnimator)
{
	m_pAnimator = pAnimator;
}

void Character::PauseAnimator()
{
	m_pAnimator->Pause();
}

void Character::UnPauseAnimator()
{
	m_pAnimator->Play();
}
