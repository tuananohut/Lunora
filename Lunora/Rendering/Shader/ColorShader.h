#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "Shader.h"

struct ColorShader
{
  BaseShader baseShader;
};

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, ColorShader* shader);


bool Render(RendererContext& RenderBuffers, ColorShader* shader, uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj);

void ReleaseShaderResources(ColorShader* shader);

#endif
