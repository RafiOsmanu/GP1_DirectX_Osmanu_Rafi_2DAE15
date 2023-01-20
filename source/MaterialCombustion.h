#pragma once
#include "Camera.h"
#include"Texture.h"
#include "BaseMaterial.h"

namespace dae {
	class MaterialCombustion : public BaseMaterial
	{
	public:
		MaterialCombustion(ID3D11Device* pDevice, const std::wstring& assetFile);
		virtual ~MaterialCombustion();

	private:

		void SetResourceVariable();
		void SetTexture(ID3D11Device* pDevice, ID3DX11EffectShaderResourceVariable* pResourceVariable, const std::string& path);

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	};
}

