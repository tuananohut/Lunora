#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "../../Engine/src/Renderer.h"

using namespace DirectX; 
using namespace LunoraEngine; 

struct BaseShader
{
  ID3D11Buffer *m_matrixBuffer = nullptr;
  ID3D11VertexShader *m_vertexShader = nullptr;
  ID3D11PixelShader *m_pixelShader = nullptr;
  ID3D11InputLayout *m_layout = nullptr;

  void Release(); 
};

struct ColorShader
{
  BaseShader baseShader;
};

struct TextureShader
{
  BaseShader baseShader; 
  ID3D11SamplerState *m_sampleState = nullptr; 
};

struct LightShader
{
  TextureShader textureShader;
  ID3D11SamplerState *m_sampleState = nullptr;
  ID3D11Buffer *m_lightBuffer = nullptr; 
};
   
struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

struct LightBufferType
{
  XMFLOAT3 AmbientDowwn;
  XMFLOAT3 AmbientRange;
};

HRESULT CompileShaderFromFile(WCHAR* filename,
			      const char* entryPoint,
			      const char* shaderModel,
			      ID3DBlob** outBlob);

HRESULT CreateMatrixBuffer(ID3D11Device* device, ID3D11Buffer** matrixBuffer);

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, ColorShader* shader);
HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader* shader);
HRESULT InitializeShaderResources(RendererContext& RenderBuffers, LightShader* shader);

bool Render(RendererContext& RenderBuffers, ColorShader* shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);
bool Render(RendererContext& RenderBuffers, TextureShader* shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture);
bool Render(RendererContext& RenderBuffers, LightShader* shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture);


void ReleaseShaderResources(ColorShader* shader);
void ReleaseShaderResources(TextureShader* shader);
void ReleaseShaderResources(LightShader* shader);

#endif
 
