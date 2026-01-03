#ifndef HEMISPHERICMESH_H
#define HEMISPHERICMESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>

#include "../Engine/src/Renderer.h"
#include "../Rendering/Texture.h"

using namespace DirectX;
using namespace LunoraEngine; 

struct HemisphericVertex
{
  XMFLOAT3 position;
  XMFLOAT2 texture;
  XMFLOAT3 normal; 
};

struct HemisphericMesh
{
  ID3D11Buffer *vertexBuffer = nullptr;
  ID3D11Buffer *indexBuffer = nullptr;
  uint32_t vertexCount = 0;
  uint32_t indexCount = 0;
  uint32_t stride = sizeof(HemisphericVertex);
  uint32_t offset = 0;
  HemisphericVertex* vertices = nullptr;
  uint32_t* indices = nullptr;
  const char *filename = nullptr;
};

bool HemisphericMeshInitialize(ID3D11Device* device, HemisphericMesh* mesh); 

bool HemisphericMeshCreateVertexBuffer(ID3D11Device* device, HemisphericMesh* mesh);
bool HemisphericMeshCreateIndexBuffer(ID3D11Device* device, HemisphericMesh* mesh);

void HemisphericMeshRender(ID3D11Device* device, HemisphericMesh* mesh);
    
void HemisphericMeshRelease(HemisphericMesh* mesh);
bool HemisphericMeshLoadFromFile(HemisphericMesh* mesh);


#endif
