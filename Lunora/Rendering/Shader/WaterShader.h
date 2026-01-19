#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "Shader.h"

struct WaterShader
{
  BaseShader baseShader;
  ID3D11SamplerState *m_sampleState = nullptr;
  ID3D11Buffer *m_waveBuffer = nullptr; 
};

struct WaveBufferType
{
  float time;
  XMFLOAT3 cameraPosition; 
}; 

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, WaterShader* shader);

bool Render(RendererContext& RenderBuffers, WaterShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture,
	    float time, XMFLOAT3 cameraPosition);

void ReleaseShaderResources(WaterShader* shader);

#endif 

