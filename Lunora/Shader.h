#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX; 

#include "Renderer.h"

static ID3D11Buffer *g_pMatrixBuffer;
static ID3D11VertexShader* m_vertexShader = nullptr;
static ID3D11PixelShader* m_pixelShader = nullptr;
static ID3D11InputLayout* layout = NULL;

struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

HRESULT InitializeShaderResources(CoreRenderBuffers& RenderBuffers);

bool Render(CoreRenderBuffers& RenderBuffers,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);

#endif
 
