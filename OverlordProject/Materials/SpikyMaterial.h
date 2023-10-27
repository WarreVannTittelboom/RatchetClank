

#pragma once
class SpikyMaterial : public Material<SpikyMaterial>
{
public:
	SpikyMaterial();
	~SpikyMaterial() override = default;
	SpikyMaterial(const SpikyMaterial& other) = delete;
	SpikyMaterial(SpikyMaterial&& other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial& other) = delete;
	SpikyMaterial& operator=(SpikyMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetfile);
	void SetNormalTexture(const std::wstring& assetfile);
	void SetEnvTexture(const std::wstring& assetfile);
	void SetOpacityTexture(const std::wstring& assetfile);
	void SetSpecularTexture(const std::wstring& assetfile);
	void onImGUI();
protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{};
	TextureData* m_pNormalTexture{};
	TextureData* m_pEnvTexture{};
	TextureData* m_pSpecularTexture{};
	TextureData* m_pOpacityTexture{};

};



