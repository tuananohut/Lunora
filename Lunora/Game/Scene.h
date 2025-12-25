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
  size_t entity_num; 
  Entity *entities[3];
  
  LARGE_INTEGER frequency;
  LARGE_INTEGER startTime;
};

bool InitializeScene(Scene &scene, Camera& mCamera, RendererContext &Renderer, HWND hwnd);
bool RenderScene(Scene &scene, Camera& mCamera, RendererContext &Renderer, MatrixBufferType& matrix);
void CleanScene(Scene *scene);

#endif
