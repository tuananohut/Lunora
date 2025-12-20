#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct Scene
{
  Camera *mCamera;
  Entity *entities[entity_num];
};

bool InitializeScene(ID3D11Device* device);
bool RenderScene();
void CleanScene();

#endif
