#include "pch.h"
#include "MaterialVehicle.h"


namespace dae
{
	MaterialVehicle::MaterialVehicle(ID3D11Device* pDevice, const std::wstring& assetFile)
		: BaseMaterial(pDevice, assetFile)
	{
		SetResourceVariable();
		
		SetTexture(pDevice, m_pDiffuseMapVariable, "Resources/vehicle_diffuse.png");
		SetTexture(pDevice, m_pDiffuseMapVariable, "Resources/vehicle_normal.png");
		SetTexture(pDevice, m_pDiffuseMapVariable, "Resources/vehicle_specular.png");
		SetTexture(pDevice, m_pDiffuseMapVariable, "Resources/vehicle_gloss.png");
	}

	MaterialVehicle::~MaterialVehicle()
	{
		m_pEffect->Release();
		m_pDiffuseMapVariable->Release();
		m_pNormalMapVariable->Release();
		m_pSpecularMapVariable->Release();
		m_pGlossinessMapVariable->Release();
	}

	void MaterialVehicle::SetResourceVariable()
	{
		//difuse map
		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid\n";
		}

		m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
		if (!m_pNormalMapVariable->IsValid())
		{
			std::wcout << L"m_pNormalMapVariable not valid\n";
		}

		//specular variable
		m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
		if (!m_pSpecularMapVariable->IsValid())
		{
			std::wcout << L"m_pSpecularMapVariable not valid\n";
		}

		m_pGlossinessMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
		if (!m_pGlossinessMapVariable->IsValid())
		{
			std::wcout << L"m_pGlossinessMapVariable not valid\n";
		}
	}

	void MaterialVehicle::SetTexture(ID3D11Device* pDevice, ID3DX11EffectShaderResourceVariable* pResourceVariable, const std::string& path)
	{
		std::unique_ptr<Texture> texture{};
		texture->LoadFromFile(pDevice, path);

		if (pResourceVariable)
		{
			pResourceVariable->SetResource(texture->GetResourceView());
		}
	}
}
