#pragma once
#include "pch.h"
#include "Math.h"
#include "vector"
#include "Effect.h"


namespace dae
{
	struct Vertex_PosCol
	{
		Vector3 position{};
		ColorRGB color{ colors::White };
		Vector2 uv{}; 
		Vector3 normal{}; 
		Vector3 tangent{};
		Vector3 viewDirection{}; 
	};

	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex_PosCol>& vertices, const std::vector<uint32_t>& indices)
			:m_pEffect{ new Effect{ pDevice, L"Resources/PosCol3D.fx" } }
			,m_pIndexBuffer{nullptr}
			,m_pVertexBuffer{nullptr}
			,m_numIndices{0}
		{

			//load Texture from file
			m_pDiffuseTexture = m_pDiffuseTexture->LoadFromFile(pDevice, "Resources/vehicle_diffuse.png");

			//set texture to effectvariable
			m_pEffect->SetDiffuseMapTexture(m_pDiffuseTexture.get());

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

			

			//Create vertex buffer
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_IMMUTABLE;
			bd.ByteWidth = sizeof(Vertex_PosCol) * static_cast<uint32_t>(vertices.size());
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = vertices.data();

			HRESULT result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
			if (FAILED(result))
				return;

			//Create Input Layout
			D3DX11_PASS_DESC passDesc{};
			m_pEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

				result = pDevice->CreateInputLayout(
				vertexDesc,
				numElements,
				passDesc.pIAInputSignature,
				passDesc.IAInputSignatureSize,
				&m_pInputLayout);

			if (FAILED(result))
				assert(false); //or return

			//Create index buffer
			m_numIndices = static_cast<uint32_t>(indices.size());
			bd.Usage = D3D11_USAGE_IMMUTABLE;
			bd.ByteWidth = sizeof(uint32_t) * m_numIndices;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			initData.pSysMem = indices.data();
			result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

			if (FAILED(result))
				return;

			
		}

		~Mesh() 
		{
			delete m_pEffect;
			m_pVertexBuffer->Release();
			m_pIndexBuffer->Release();
			m_pInputLayout->Release();
			
		}

	

		void Render(ID3D11DeviceContext* pDeviceContext, Camera* pCamera)
		{
			//1. Set PrimitiveTopolgy
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//2. Set Input Layout
			pDeviceContext->IASetInputLayout(m_pInputLayout);

			//3. Set VertexBuffer
			constexpr UINT stride = sizeof(Vertex_PosCol);
			constexpr UINT offset = 0;
			pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

			//4. Set IndexBuffer
			pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//5. update worldViewProjectMatrix
			Matrix worldViewProjectionMatrix{ worldMatrix * pCamera->GetViewMatrix() * pCamera->GetProjectionMatrix()};
			m_pEffect->SetMatrix(worldViewProjectionMatrix);

			//6. Draw
			D3DX11_TECHNIQUE_DESC techDesc{};
			//from my own Effect Class
			m_pEffect->GetTechnique()->GetDesc(&techDesc);

			//for(UINT p = 0; p < techDesc.Passes; ++p)
			{
				//from my own Effect Class
				m_pEffect->GetTechnique()->GetPassByIndex(pass)->Apply(0, pDeviceContext);
				pDeviceContext->DrawIndexed(m_numIndices, 0, 0);
			}

		}
		void TogglePass()
		{
			pass = (pass + 1) % 3;
		}

		Matrix worldMatrix{};
	private:
		Effect* m_pEffect;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		ID3D11InputLayout* m_pInputLayout;
		uint32_t m_numIndices;
		std::unique_ptr<Texture> m_pDiffuseTexture{ nullptr };
		UINT pass{0};

	};
}
