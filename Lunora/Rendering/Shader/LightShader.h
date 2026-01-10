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
    XMFLOAT4 AmbientDown;
    XMFLOAT4 AmbientUp;  
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, LightShader* shader);

bool Render(RendererContext& RenderBuffers, LightShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture, XMFLOAT4 AmbientDown, XMFLOAT4 AmbientRange);

void ReleaseShaderResources(LightShader* shader);

#endif 
