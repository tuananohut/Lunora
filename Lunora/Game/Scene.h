#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "Camera/Camera.h"
#include "../Engine/src/Renderer.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Shader/Shader.h"
#include "../Rendering/Texture.h"

using namespace LunoraEngine;

const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

static bool Running;

struct Scene
{
  Camera *mCamera;
  size_t entity_num; 
  Entity *entities[3];
  RendererContext *Renderer;
  
  LARGE_INTEGER frequency;
  LARGE_INTEGER startTime;
};

bool InitializeScene(Scene *scene, HWND hwnd);
bool RenderScene(Scene *scene);
void CleanScene(Scene *scene);

#endif
