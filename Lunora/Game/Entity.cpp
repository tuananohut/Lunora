#include "Entity.h"

bool InitializeEntity(Entity* Entity, ID3D11Device* device)
{
  size_t entity_num = sizeof(Entity) / sizeof(Entity[0]);
  bool running; 
  
  for (size_t i = 0; i < entity_num; i++)
    {
      InitializeModel(device, Entity[i].mesh);
    } 
  
  return true; 
}

bool RenderEntity(Entity* Entity)
{
  return true;
}

void ReleaseEntity(Entity* Entity) {}
