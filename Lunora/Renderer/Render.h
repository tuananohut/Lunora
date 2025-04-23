#ifndef RENDER_H
#define RENDER_H

#include "../Core/Device/DeviceManager.h"
#include "../Scene/Scene.h"
#include "RenderData.h"
#include "Transform/TransformSystem.h"

static void RenderCube(DeviceManager& DeviceManager, 
		       ShaderGPUData* Shader, 
		       MeshGPUData* Mesh,
		       XMMATRIX WorldMatrix,
		       XMMATRIX ViewMatrix,
		       XMMATRIX ProjectionMatrix);

void RenderScene(DeviceManager& DeviceManager,
		 ShaderGPUData* Shader,
		 Scene* Scene,
		 XMMATRIX ViewMatrix,
		 XMMATRIX ProjectionMatrix);

#endif
