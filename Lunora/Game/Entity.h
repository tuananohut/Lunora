#ifndef ENTITY_H
#define ENTITY_H

#include "../Rendering/Mesh.h"
#include "../Rendering/Shader/Shader.h"
#include "../Engine/Renderer.h"

struct Entity
{
  Mesh* mesh;
  ColorShader* color_shader;
  TextureShader* texture_shader;   
};

bool InitializeEntity(Entity* Entity, size_t entity_num, RendererContext& RenderBuffers);
bool RenderEntity(Entity* Entity);
void ReleaseEntity(Entity* Entity, size_t entity_num);

#endif
