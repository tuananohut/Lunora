#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <WinUser.h>
#include <wrl/client.h>

#include <windows.h>
#include <d2d1.h>

struct RenderManager
{ 
	RenderManager()
	{
		D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 };
		D3D_FEATURE_LEVEL m_featureLevel = levels[0];
		UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		HRESULT hr;

#if defined(DEBUG) || defined(_DEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif;

		// ID3D11Device* device;
		// ID3D11DeviceContext* deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

		hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			deviceFlags,
			levels,
			ARRAYSIZE(levels),
			D3D11_SDK_VERSION,
			&device,
			&m_featureLevel,
			&deviceContext);
		
		if (SUCCEEDED(hr))
		{
			OutputDebugStringA("You are genius");
		}
		else
		{
			OutputDebugStringA("Iditiota");
		}

	}

	~RenderManager() {}
};

struct RenderManager2D
{
	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* RenderTarget;
	ID2D1SolidColorBrush* Brush;
	D2D1_ELLIPSE ellipse;
	HWND Window; 

	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    OnPaint();
	void    Resize();
	void    Shutdown(); 
	void	GetHwnd(HWND hwnd);
};

#endif