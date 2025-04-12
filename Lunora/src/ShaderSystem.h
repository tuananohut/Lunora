#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include "RenderData.h"
#include "Managers.h"
#include <d3dcompiler.h>

bool LoadShader(ID3D11Device* Device,
		const wchar_t* vsPath,
		const wchar_t* psPath,
		ShaderData* outShader,
		RenderManager* Renderer);

void UseShader(ID3D11DeviceContext* DeviceContext,
	       const ShaderData* Shader);

void ReleaseShader(ShaderData* Shader);

#endif
