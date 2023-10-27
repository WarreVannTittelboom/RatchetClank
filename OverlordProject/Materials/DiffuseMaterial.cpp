#include "stdafx.h"
#include "DiffuseMaterial.h"


DiffuseMaterial::DiffuseMaterial() :
	Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial::InitializeEffectVariables()
{
}
