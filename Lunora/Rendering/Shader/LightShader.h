#ifndef LIGHTSHADER_H
#define LIGHTSHADER_H

#include "Shader.h"

struct LightShader
{
  BaseShader baseShader;
  ID3D11SamplerState *m_sampleState = nullptr;
  ID3D11Buffer *m_lightBuffer = nullptr; 
};

struct LightBufferType
{
  XMFLOAT3 AmbientDown;
  XMFLOAT3 AmbientRange;
  float padding1;
  float padding2; 
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, LightShader* shader);

bool Render(RendererContext& RenderBuffers, LightShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture, XMFLOAT3 AmbientDown, XMFLOAT3 AmbientRange);

void ReleaseShaderResources(LightShader* shader);

#endif 
