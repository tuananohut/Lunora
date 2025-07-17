#ifndef MESHDATA
#define MESHDATA

#include <d3d11.h>
#include <DirectXMath.h>

#include "../renderer.h"

using namespace DirectX; 

struct Mesh
{
  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT2 texture;
    XMFLOAT3 normal;
    XMFLOAT3 color; 
  };
  
  struct ModelType
  {
    float x, y, z;
    float tu, tv;
    float nx, ny, nz;
    float r, g, b; 
  };

  struct Vec3
  {
    float x, y, z; 
  };

  virtual bool InitializeBuffers(ID3D11Device*);
  virtual void ShutdownBuffers();
  virtual void RenderBuffers(ID3D11DeviceContext*);
};




#endif
