#ifndef ENTITY_H
#define ENTITY_H

#include <DirectXMath.h>

#include "../Rendering/Mesh.h"
#include "../Rendering/HemisphericMesh.h"
#include "../Rendering/Shader/WaterShader.h"
#include "../Rendering/Texture.h"
#include "../Engine/src/Renderer.h"

struct Transform
{
  XMFLOAT3 position;
  XMFLOAT3 rotation;
  XMFLOAT3 scale;
};

struct Entity
{
  Transform transform; 
  XMMATRIX worldMatrix;
  
  HemisphericMesh hemisphericMesh;   
  WaterShader water_shader; 
  Texture texture; 
};

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers);
bool RenderEntity(RendererContext& Renderer, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time);

XMMATRIX ComputeWorldMatrix(const Transform& t);

#endif
