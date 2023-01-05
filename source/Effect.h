#pragma once
#include "Camera.h"
#include"Texture.h"
namespace dae {
	class Effect
	{
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();
		ID3DX11EffectTechnique* GetTechnique();
		void SetMatrix(const Matrix& worldViewProjectMatrix);
		void SetDiffuseMapTexture(Texture* pDiffuseTexture);


	private:
		ID3DX11Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTechnique;
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
		Matrix m_WorldViewProjecMatrix;
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	};
}

