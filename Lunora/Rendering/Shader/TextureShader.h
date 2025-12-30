#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "Shader.h"

struct TextureShader
{
  BaseShader baseShader; 
  ID3D11SamplerState *m_sampleState = nullptr; 
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader* shader);

bool Render(RendererContext& RenderBuffers, TextureShader* shader, UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture);

void ReleaseShaderResources(TextureShader* shader);

#endif
