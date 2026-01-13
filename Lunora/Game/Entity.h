#ifndef ENTITY_H
#define ENTITY_H

#include <DirectXMath.h>

#include "../Rendering/Mesh.h"
#include "../Rendering/HemisphericMesh.h"
#include "../Rendering/Shader/Shader.h"
#include "../Rendering/Shader/LightShader.h"
#include "../Rendering/Shader/AmbientLightShader.h"
#include "../Rendering/Shader/TextureShader.h"
#include "../Rendering/Shader/ColorShader.h"
#include "../Rendering/Texture.h"
#include "../Engine/src/Renderer.h"

struct Transform
{
  XMFLOAT3 position;
  XMFLOAT3 rotation;
  XMFLOAT3 scale;
};

struct MeshManager
{
  Mesh mesh;
  HemisphericMesh hemisphericMesh; 
};

struct MaterialManager
{
  ColorShader color_shader;
  TextureShader texture_shader;
  LightShader light_shader;
  AmbientLightShader ambient_light_shader;
  Texture texture; 
};

struct Entity
{
  Transform transform; 
  XMMATRIX worldMatrix;

  MeshManager meshes;
  MaterialManager materials; 
};

bool InitializeEntity(Entity* Entity[], size_t entity_num, RendererContext& RenderBuffers);
bool RenderEntity(RendererContext& Renderer, Entity* Entity[], size_t entity_num, MatrixBufferType& matrix, float total_time);

XMMATRIX ComputeWorldMatrix(const Transform& t);

#endif
