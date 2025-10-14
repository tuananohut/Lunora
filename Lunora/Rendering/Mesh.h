#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "Renderer.h"
#include "ModelLoader.h"

using namespace DirectX; 

struct Vertex
{
  XMFLOAT3 position;
  XMFLOAT2 texCoord;
  XMFLOAT3 normal; 
};

struct Mesh 
{  
  ID3D11Buffer *vertexBuffer = nullptr;
  ID3D11Buffer *indexBuffer = nullptr;
  UINT vertexCount = 0;
  UINT indexCount = 0;
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
};

bool MeshInitialize(Mesh &mesh,
		    ID3D11Device *device,
		    const Vertex* vertices,
		    UINT vertexCount,
		    const unsigned long* indices,
		    UINT indexCount);

HRESULT CreateVertexBuffer(ID3D11Device *Device, SimpleVertexCombined* vertices,
			   int vertexCount, ID3D11Buffer** VertexBuffer);
HRESULT CreateIndexBuffer(ID3D11Device *Device, unsigned long* indices,
			  int indexCount, ID3D11Buffer** IndexBuffer);

bool InitializeModel(CoreRenderBuffers& RenderBuffers, ModelBuffer* Buffer,
		     char filename[]); 

void RenderModel(CoreRenderBuffers& RenderBuffers, ModelBuffer& Buffer); 

void ReleaseModel(ModelBuffer& Buffer); 

#endif
