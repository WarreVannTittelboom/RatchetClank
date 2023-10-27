#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color) :
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetVisible(bool visible)
{
	m_Visible = visible;
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& /*sceneContext*/)
{
	if (!m_pTexture || !m_Visible)
		return;

	XMFLOAT2 pos = { m_pGameObject->GetTransform()->GetWorldPosition().x, m_pGameObject->GetTransform()->GetWorldPosition().y };
	XMFLOAT2 scale = { m_pGameObject->GetTransform()->GetScale().x, m_pGameObject->GetTransform()->GetScale().y };
	float rot = MathHelper::QuaternionToEuler(m_pGameObject->GetTransform()->GetWorldRotation()).z;
	float depth = m_pGameObject->GetTransform()->GetPosition().z;
	SpriteRenderer::Get()->AppendSprite(m_pTexture, pos, m_Color, m_Pivot, scale, rot, depth);
}
