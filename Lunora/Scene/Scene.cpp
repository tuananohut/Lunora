#include <Windows.h>

#include "Scene.h"

void AddMesh(MeshList* list, MeshGPUData* mesh)
{
  if (list->Count < MAX_MESH_COUNT)
    {
      list->Meshes[list->Count++] = mesh; 
    }
  else
    {
      OutputDebugStringA("MeshList is full.\n"); 
    }
}

void RemoveMesh(MeshList* list, int index)
{
  if (index < 0 || index >= list->Count)
    {
      return; 
    }

  for (int i = index; i < list->Count-1; i++)
    {
      list->Meshes[i] = list->Meshes[i + 1]; 
    }

  list->Count--; 
}
