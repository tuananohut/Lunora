#ifndef AMBIENTLIGHTSHADER_H
#define AMBIENTLIGHTSHADER_H

#include "Shader.h"

struct AmbientLightShader
{
  BaseShader baseShader;
  ID3D11SamplerState *m_sampleState = nullptr;
  ID3D11Buffer *m_lightBuffer = nullptr; 
};

struct AmbientLightBufferType
{
  XMFLOAT4 ambientColor;
  XMFLOAT4 diffuseColor;
  XMFLOAT3 lightDirection;
  float padding;
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, AmbientLightShader* shader);

bool Render(RendererContext& RenderBuffers, AmbientLightShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection);

void ReleaseShaderResources(AmbientLightShader* shader);

#endif 
