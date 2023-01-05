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
		LoadInputLayout(pDevice, m_pTechnique);

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
		//m_pTechnique->Release();
		m_pInputLayout->Release();
		delete m_pCamera;
		m_pDiffuseMapVariable->Release();
	}

	ID3D11InputLayout* Effect::GetInputLayout()
	{
		return m_pInputLayout;
	}

	ID3DX11EffectTechnique* Effect::GetTechnique()
	{
		return m_pTechnique;
	}

	ID3D11InputLayout* Effect::LoadInputLayout(ID3D11Device* pDevice, ID3DX11EffectTechnique* pTechnique)
	{
		//Create Vertex Layout
		static constexpr uint32_t numElements{ 3 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};
		vertexDesc[0].SemanticName = "POSITION";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		vertexDesc[0].AlignedByteOffset = 0;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "COLOR";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		vertexDesc[1].AlignedByteOffset = 12;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "TEXCOORD";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		vertexDesc[2].AlignedByteOffset = 24;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		//Create Input Layout
		D3DX11_PASS_DESC passDesc{};
		m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

		const HRESULT result = pDevice->CreateInputLayout(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pInputLayout);

		if (FAILED(result))
			assert(false); //or return

		return m_pInputLayout;
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

