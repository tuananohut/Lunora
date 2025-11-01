#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX; 

#include "../../Engine/Renderer.h"

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
   
struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

HRESULT CompileShaderFromFile(WCHAR* filename,
			      const char* entryPoint,
			      const char* shaderModel,
			      ID3DBlob** outBlob);

HRESULT CreateMatrixBuffer(ID3D11Device* device, ID3D11Buffer** matrixBuffer);

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, ColorShader& shader);
HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader& shader);

bool Render(RendererContext& RenderBuffers, ColorShader& shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);
bool Render(RendererContext& RenderBuffers, TextureShader& shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture);

void ReleaseShaderResources(ColorShader& shader);
void ReleaseShaderResources(TextureShader& shader);

#endif
 
