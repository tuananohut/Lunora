#ifndef HEXMESH_H
#define HEXMESH_H

#include <DirectXMath.h>

using namespace DirectX; 

struct HexMesh: public MeshData
{
  struct HeightMap
  {
    float x, y, z;
    float nx, ny, nz;
    float r, g, b; 
  };

  HexMesh();
  ~HexMesh();

  bool Initialize(ID3D11Device*);
  void Shutdown();
  void Render(ID3D11DeviceContext*);

  
};

#endif
