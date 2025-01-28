#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <WinUser.h>
#include <wrl/client.h>

HWND hwnd;

class RenderManager
{
public: 
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

		if (FAILED(hr))
		{
			MessageBox(hwnd, L"Error", L"Could not create device", MB_OK | MB_ICONERROR); 
		}

		MessageBox(hwnd, L"Well done!", L"Device created!", MB_OK | MB_ICONINFORMATION); 
	
	}

	~RenderManager() {}
};


#endif