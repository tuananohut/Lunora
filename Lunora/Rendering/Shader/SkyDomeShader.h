#ifndef SKYDOMESHADER_H
#define SKYDOMESHADER_H

#include "Shader.h"

struct SkyDomeShader
{
  BaseShader baseShader;
  ID3D11Buffer *m_colorBuffer = nullptr; 
};

struct SkyDomeBufferType
{
  XMFLOAT4 apexColor; 
  XMFLOAT4 centerColor;
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, SkyDomeShader* shader);

bool Render(RendererContext& RenderBuffers, SkyDomeShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    XMFLOAT4 apexColor, XMFLOAT4 centerColor);

void ReleaseShaderResources(SkyDomeShader* shader);

#endif
