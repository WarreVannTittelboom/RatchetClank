#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	m_ClipSet = false;
	for (auto clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clip.name == clipName)
		{
			SetAnimation(clip);
			return;
		}
	}
	Reset();
	Logger::LogWarning(L"Clip with corresponding name could not be found.");
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;

	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
		m_CurrentClipNumber= clipNumber;
	}
	else
	{
		Reset();
		Logger::LogWarning(L"Clip with corresponding number could not be found.");
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;

	Reset(false);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	if (m_IsPlaying && m_ClipSet)
	{
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0.f)
				m_TickCount += m_CurrentClip.duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
				m_TickCount -= m_CurrentClip.duration;
		}
		AnimationKey keyA{ m_CurrentClip.keys.front() }, keyB{ m_CurrentClip.keys.back() };
		for (size_t i{}; i < m_CurrentClip.keys.size(); ++i)
		{
			if (m_CurrentClip.keys[i].tick < m_TickCount && m_CurrentClip.keys[i].tick > keyA.tick) {keyA = m_CurrentClip.keys[i];}
			if (m_CurrentClip.keys[i].tick > m_TickCount && m_CurrentClip.keys[i].tick < keyB.tick) {keyB = m_CurrentClip.keys[i];}
		}
		auto blendFactor{ (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick) };
		const size_t boneTransformCount{ keyA.boneTransforms.size() };
		m_Transforms.clear();
		m_Transforms.resize(boneTransformCount);
		for (size_t i{}; i < boneTransformCount; ++i)
		{
			auto transformA{ keyA.boneTransforms[i] };
			XMMATRIX matA{ DirectX::XMLoadFloat4x4(&transformA) };
			XMVECTOR scaleA{}, rotA{}, transA{};
			XMMatrixDecompose(&scaleA, &rotA, &transA, matA);
			auto transformB{ keyB.boneTransforms[i] };
			XMMATRIX matB{ DirectX::XMLoadFloat4x4(&transformB) };
			XMVECTOR scaleB{}, rotB{}, transB{};
			XMMatrixDecompose(&scaleB, &rotB, &transB, matB);
			XMVECTOR scaleL{ XMVectorLerp(scaleA, scaleB, blendFactor) };
			XMVECTOR rotL{ XMQuaternionSlerp(rotA, rotB, blendFactor) };
			XMVECTOR transL{ XMVectorLerp(transA, transB, blendFactor) };
			XMStoreFloat4x4(&(m_Transforms[i]), XMMatrixAffineTransformation(scaleL, XMVECTOR{}, rotL, transL));
		}
	}
}

void ModelAnimator::Reset(bool pause)
{
	if (pause) { m_IsPlaying = false; }
	m_TickCount = 0.f;
	m_AnimationSpeed = 1.0f;
	if (m_ClipSet)
	{
		// new bone transform
		m_Transforms.assign(m_CurrentClip.keys[0].boneTransforms.begin(), m_CurrentClip.keys[0].boneTransforms.end());
	}
	else
	{
		// identity matrix
		XMFLOAT4X4 identityMatrix{};
		XMStoreFloat4x4(&identityMatrix, XMMatrixIdentity());
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identityMatrix);
	}
}