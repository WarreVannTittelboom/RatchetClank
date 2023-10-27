#include "stdafx.h"
#include "UberMaterial.h"



UberMaterial::UberMaterial() :
	Material<UberMaterial>(L"Effects/UberShader.fx")
{}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pNormalTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularTexture);
}



void UberMaterial::SetEnvTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pEnvTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gCubeEnvironment", m_pEnvTexture);
}


void UberMaterial::SetOpacityTexture(const std::wstring& assetfile)
{
	//m_pDiffuseTexture DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetfile);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);
}


void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gFlipGreenChannel", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	SetVariable_Scalar(L"gUseSpecularPhong", true);
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", true);
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT3{ Colors::Purple });

}

void UberMaterial::onImGUI()
{
	BaseMaterial::DrawImGui();
}
	