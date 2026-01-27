#ifndef SKYDOME_H
#define SKYDOME_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "../Engine/src/Renderer.h"
#include "../Rendering/Texture.h"

using namespace LunoraEngine; 
using namespace DirectX;
using namespace std;

struct ModelType
{
  float x, y, z;
  float tu, tv;
  float nx, ny, nz;
};

struct SkyDomeVertex
{
  XMFLOAT3 position; 
};

struct SkyDomeMesh
{
  ID3D11Buffer *vertexBuffer = nullptr;
  ID3D11Buffer *indexBuffer = nullptr;
  uint32_t vertexCount = 0;
  uint32_t indexCount = 0;
  uint32_t stride = sizeof(SkyDomeVertex);
  uint32_t offset = 0;
  SkyDomeVertex* vertices = nullptr;
  uint32_t* indices = nullptr;
  char *filename;
};

bool SkyDomeMeshInitialize(ID3D11Device* device, SkyDomeMesh* mesh); 
     
bool SkyDomeMeshCreateVertexBuffer(ID3D11Device* device, SkyDomeMesh* mesh);
bool SkyDomeMeshCreateIndexBuffer(ID3D11Device* device, SkyDomeMesh* mesh);
     
void SkyDomeMeshRender(RendererContext& RenderBuffers, SkyDomeMesh* mesh);
     
void SkyDomeMeshRelease(SkyDomeMesh* mesh);
bool SkyDomeMeshLoadFromFile(SkyDomeMesh* mesh);

#endif
