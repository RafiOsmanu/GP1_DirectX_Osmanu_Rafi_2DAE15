#pragma once
#include <SDL_surface.h>
#include <string>
#include "ColorRGB.h"
#include<memory>
#include "Vector3.h"

namespace dae
{
	struct Vector2;

	class Texture
	{
	public:
		~Texture();
		std::unique_ptr<Texture> ResourceCreation(ID3D11Device*& pDevice, const std::string& path);
		ID3D11ShaderResourceView* GetResourceView();
		Texture(SDL_Surface* pSurface);

	private:

		SDL_Surface* m_pSurface{ nullptr };
		uint32_t* m_pSurfacePixels{ nullptr };
		ID3D11Texture2D* m_pResource;
		ID3D11ShaderResourceView* m_pSRV;
	};
}

