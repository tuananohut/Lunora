#ifndef TERRAINMESHDATA
#define TERRAINMESHDATA

#include <d3d11.h>
#include <DirectXMath.h>

#include "../MeshData.h"

using namespace DirectX;

struct Terrain: public Mesh
{
  Terrain();
  ~Terrain();

  bool Initialize(ID3D11Device*, char*);
  void Shutdown();
  bool Render(ID3D11DeviceContext*);

  int GetIndexCount();

  virtual bool InitializeBuffers(ID3D11Device*);
  virtual void ShutdownBuffers();
  virtual void RenderBuffers(ID3D11DeviceContext*);
 
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;
};

#endif
