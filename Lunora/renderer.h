#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include "Core/Device/DeviceManager.cpp"
#include "Core/Pipeline/PipelineStateManager.cpp"
#include "Core/Resource/RenderTargetManager.cpp"

#include "Camera/Camera.h"
#include "Camera/Camera.cpp"

#include "Renderer/Transform/Transform.h"
#include "Renderer/Transform/TransformSystem.h"

using namespace DirectX;

#define DeleteObject(object) if ((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if ((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object) if ((object) != NULL) { object->Release(); object = NULL; }

const UINT ScreenWidth = 1280;
const UINT ScreenHeight = 720;

static XMMATRIX GetWorldMatrix(const Transform& T)
{
  XMMATRIX world = T.Scale * T.RotationMatrix * T.Translation;

  return world; 
} 

static XMMATRIX WorldMatrix = XMMatrixIdentity();
static XMMATRIX ViewMatrix = XMMatrixLookAtLH
  (
   XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f),
   XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
   XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
   );
static XMMATRIX ProjectionMatrix = XMMatrixPerspectiveFovLH
  (
   XM_PIDIV4,
   ScreenWidth / ScreenHeight,
   0.1f,
   1000.0f
   );

static FLOAT BackgroundColor[4] = {0.141f, 0.137f, 0.365f, 1.f};

// Default colors
static LPCWSTR FileNames[3][2] = {{L"../Lunora/assets/color.vs", L"../Lunora/assets/color.ps"},
				  {L"../Lunora/assets/texture.vs", L"../Lunora/assets/texture.ps"},
				  {L"../Lunora/assets/light.vs", L"../Lunora/assets/light.ps"}};

static LPCWSTR VSFileName = FileNames[2][0];
static LPCWSTR PSFileName = FileNames[2][1];

static void InitializeDX11(HWND Window);
static void CreateCube(HWND Window, LPCWSTR VSFilename, LPCWSTR PSFilename);

ID3DBlob* CompileShader(const LPCWSTR& shaderPath,
			const LPCSTR& entryPoint,
			const LPCSTR& target)
{
  ID3DBlob* shaderBlob = nullptr;
  ID3DBlob* errorBlob = nullptr;
  HRESULT hr = D3DCompileFromFile(shaderPath,
				  nullptr,
				  nullptr,
				  entryPoint,
				  target,
				  D3DCOMPILE_DEBUG,
				  0,
				  &shaderBlob,
				  &errorBlob);
  return shaderBlob;
}


#endif
