#ifndef ENTITY_H
#define ENTITY_H

#include "../Rendering/Mesh.h"
#include "../Rendering/Shader/Shader.h"

struct Entity
{
  Mesh* mesh;
  ColorShader* color_shader;
  TextureShader* texture_shader;

   
};

bool InitializeEntity(Entity* Entity);
bool RenderEntity(Entity* Entity);
void ReleaseEntity(Entity* Entity);

#endif
