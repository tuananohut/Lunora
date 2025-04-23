#ifndef SCENE_H
#define SCENE_H

#include "../Renderer/RenderData.h"

#define MAX_MESH_COUNT 128

struct MeshList
{
  MeshGPUData* Meshes[MAX_MESH_COUNT];
  int Count;
};

void AddMesh(MeshList* list, MeshGPUData* mesh);
void RemoveMesh(MeshList* list, int index);

struct Scene
{
  MeshList Meshes;
};

#endif
