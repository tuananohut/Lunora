#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>

#include "../Engine/Renderer.h"

using namespace DirectX; 

struct Vertex
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};

struct Mesh 
{  
  ID3D11Buffer *vertexBuffer = nullptr;
  ID3D11Buffer *indexBuffer = nullptr;
  UINT vertexCount = 0;
  UINT indexCount = 0;
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  Vertex* vertices;
  unsigned long* indices; 
};

bool MeshInitialize(Mesh* Mesh,
		    ID3D11Device *device);

HRESULT CreateVertexBuffer(ID3D11Device *Device, Mesh* Mesh);
HRESULT CreateIndexBuffer(ID3D11Device *Device, Mesh* Mesh);

bool InitializeModel(ID3D11Device *Device, Mesh* Buffer,
		     char filename[]); 

void RenderModel(RendererContext& context, Mesh* Buffer); 

void ReleaseModel(Mesh* Buffer); 

bool LoadModelFromFile(char filename[], Mesh* Buffer);

#endif
