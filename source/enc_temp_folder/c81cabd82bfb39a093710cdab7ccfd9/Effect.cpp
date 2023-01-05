#include "pch.h"
#include "Effect.h"
#include <assert.h>

namespace dae 
{
	Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		m_pCamera = new Camera{ { 0.f, 0.f, -10.f }, 45.f };

		//Load the effect and store in data member
		m_pEffect = LoadEffect(pDevice, assetFile);

		//get Technique and store in data member

		//m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
		m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
		if (!m_pTechnique->IsValid())
		{
			std::wcout << L"Technique not valid\n";
		}
		m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
		if (!m_pMatWorldViewProjVariable->IsValid())
		{
			std::wcout << L"m_pMatWorldViewProjVariable not valid\n";
		}

		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid\n";
		}
	}

	Effect::~Effect()
	{
		m_pEffect->Release();
		delete m_pCamera;
	}


	ID3DX11EffectTechnique* Effect::GetTechnique()
	{
		return m_pTechnique;
	}

	void Effect::Update(const Timer* pTimer, const Matrix worldMatrix)
	{
		Matrix worldViewProjectMatrix{ worldMatrix * m_pCamera->GetViewProjecMatrix() };
		m_pCamera->Update(pTimer);
		m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProjectMatrix));
	}

	void Effect::SetDiffuseMapTexture(Texture* pDiffuseTexture)
	{
		if (m_pDiffuseMapVariable)
		{
			m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetResourceView());
		}
	}

	ID3DX11Effect* Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect;

		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		result = D3DX11CompileEffectFromFile(assetFile.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&pEffect,
			&pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); ++i)
					ss << pErrors[i];

				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;

				std::wcout << ss.str() << std::endl;

			}
			else
			{
				std::wstringstream ss;
				ss << "EffectLoader: Failed to CReateEffectFile!\npath: " << assetFile;
				std::wcout << ss.str() << std::endl;
				return nullptr;
			}
		}
		return pEffect;
	}
}

