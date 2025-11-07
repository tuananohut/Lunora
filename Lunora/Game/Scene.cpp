#include "Scene.h"

bool InitializeScene(ID3D11Device* device)
{
  Entity Entities[100]; 
  
  Entities[0].mesh = new Mesh; 
  Entities[0].mesh->filename = "../Assets/Models/triangle.txt"; 
  
  InitializeEntity(Entities, device); 
  
  return true;
}
