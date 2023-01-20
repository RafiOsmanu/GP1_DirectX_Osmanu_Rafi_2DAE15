#include "pch.h"
#include "MaterialCombustion.h"

namespace dae
{
	MaterialCombustion::MaterialCombustion(ID3D11Device* pDevice, const std::wstring& assetFile)
		:BaseMaterial(pDevice, assetFile)
	{
		SetResourceVariable();

		SetTexture(pDevice, m_pDiffuseMapVariable, "Resources/fireFX_diffuse.png");
	}

	MaterialCombustion::~MaterialCombustion()
	{
	}

	void MaterialCombustion::SetResourceVariable()
	{
		//difuse map
		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid\n";
		}
	}

	void MaterialCombustion::SetTexture(ID3D11Device* pDevice, ID3DX11EffectShaderResourceVariable* pResourceVariable, const std::string& path)
	{
		if (pResourceVariable)
		{
			pResourceVariable->SetResource(Texture::LoadFromFile(pDevice, path)->GetResourceView());
		}
	}
}
