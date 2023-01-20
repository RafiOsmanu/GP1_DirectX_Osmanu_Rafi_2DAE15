#pragma once
#include "Camera.h"
#include"Texture.h"
#include "BaseMaterial.h"

namespace dae {

	class MaterialVehicle : public BaseMaterial
	{
	public:
		MaterialVehicle(ID3D11Device* pDevice, const std::wstring& assetFile);
		virtual ~MaterialVehicle();


	private:

		void SetResourceVariable();
		void SetTexture(ID3D11Device* pDevice, ID3DX11EffectShaderResourceVariable* pResourceVariable, const std::string& path);
		
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
		ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;
	};
}


