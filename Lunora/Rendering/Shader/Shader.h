#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX; 

#include "../../Engine/Renderer.h"

struct ColorShader
{
  ID3D11Buffer *m_matrixBuffer = nullptr;
  ID3D11VertexShader *m_vertexShader = nullptr;
  ID3D11PixelShader *m_pixelShader = nullptr;
  ID3D11InputLayout *m_layout = nullptr;
};

struct TextureShader
{
  ID3D11Buffer *m_matrixBuffer = nullptr;
  ID3D11VertexShader *m_vertexShader = nullptr;
  ID3D11PixelShader *m_pixelShader = nullptr;
  ID3D11InputLayout *m_layout = nullptr;
  ID3D11SamplerState *m_sampleState = nullptr; 
};
  
struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, ColorShader& shader);
HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader& shader);

bool Render(RendererContext& RenderBuffers, ColorShader& shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);
bool Render(RendererContext& RenderBuffers, TextureShader& shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);

void ReleaseShaderResources(ColorShader& shader);
void ReleaseShaderResources(TextureShader& shader);

#endif
 
