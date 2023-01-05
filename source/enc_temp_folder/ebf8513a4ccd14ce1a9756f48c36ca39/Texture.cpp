#include "pch.h"
#include "Texture.h"
#include "Vector2.h"
#include <SDL_image.h>
#include <memory>
#include <iostream>

namespace dae
{
	Texture::Texture(SDL_Surface* pSurface) :
		m_pSurface{ pSurface },
		m_pSurfacePixels{ (uint32_t*)pSurface->pixels },
		m_pResource{nullptr},
		m_pSRV{nullptr}
	{
	}

	Texture::~Texture()
	{
		if (m_pSurface)
		{
			SDL_FreeSurface(m_pSurface);
			m_pSurface = nullptr;
		}
		if(m_pResource)
		m_pResource->Release();
		if(m_pSRV)
		m_pSRV->Release();
	}

	std::unique_ptr<Texture> Texture::LoadFromFile(const std::string& path)
	{
		//TODO
		//Load SDL_Surface using IMG_LOAD
		SDL_Surface* pData = IMG_Load(path.c_str());

		//Create & Return a new Texture Object (using SDL_Surface)
		if (pData == nullptr)
		{
			std::cout << "surface is nullptr" << "\n";
		}

		return std::make_unique<Texture>(pData);

	}

	void Texture::ResourceCreation(ID3D11Device* pDevice)
	{

		//Resource
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = m_pSurface->w;
		desc.Height = m_pSurface->h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_pSurface->pixels;
		initData.SysMemPitch = static_cast<UINT>(m_pSurface->pitch);
		initData.SysMemSlicePitch = static_cast<UINT>(m_pSurface->h * m_pSurface->pitch);

		HRESULT hr = pDevice->CreateTexture2D(&desc, &initData, &m_pResource);

		// resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
		SRVDesc.Format = format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = pDevice->CreateShaderResourceView(m_pResource, &SRVDesc, &m_pSRV);
		SDL_FreeSurface(m_pSurface);

		

	}

	ID3D11ShaderResourceView* Texture::GetResourceView()
	{
		return m_pSRV;
	}

	ColorRGB Texture::Sample(const Vector2& uv) const
	{
		//TODO
		//Sample the correct texel for the given uv
		Uint8 r, g, b;

		const Uint32 U{ static_cast<Uint32>(uv.x * m_pSurface->w) };
		const Uint32 V{ static_cast<Uint32>(uv.y * m_pSurface->h) };


		const Uint32 pixel{ m_pSurfacePixels[static_cast<Uint32>(U + (V * m_pSurface->w))] };

		SDL_GetRGB(pixel, m_pSurface->format, &r, &g, &b);

		const float colorRemap{ 1 / 255.f };


		return { r * colorRemap, g * colorRemap, b * colorRemap };
	}

	Vector3 Texture::SampleNormal(const Vector2& uv) const
	{
		//TODO
		//Sample the correct texel for the given uv
		Uint8 r, g, b;

		const Uint32 U{ static_cast<Uint32>(uv.x * m_pSurface->w) };
		const Uint32 V{ static_cast<Uint32>(uv.y * m_pSurface->h) };


		const Uint32 pixel{ m_pSurfacePixels[static_cast<Uint32>(U + (V * m_pSurface->w))] };

		SDL_GetRGB(pixel, m_pSurface->format, &r, &g, &b);

		const float colorRemap{ 1 / 255.f };


		return { r * colorRemap, g * colorRemap, b * colorRemap };
	}
}
