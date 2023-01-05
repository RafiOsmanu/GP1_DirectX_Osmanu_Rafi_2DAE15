#pragma once
#include "DataTypes.h"
#include "Texture.h"
#include "Camera.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;
		void ToggleTechniquePass();

	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		//create device and DeviceContext
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;

		//create SwapChain
		IDXGISwapChain* m_pSwapChain{};

		//texture2D
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		//render targetBuffer
		ID3D11Resource* m_pRenderTargetBuffer;

		//render targetView
		ID3D11RenderTargetView* m_pRenderTargetView;
		std::unique_ptr<Camera> m_pCamera;

		Mesh* m_pMesh;

		
		


		//DIRECTX
		HRESULT InitializeDirectX();
		//...
	};
}
