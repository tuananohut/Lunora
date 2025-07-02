#ifndef MESHDATA
#define MESHDATA

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX; 

struct Mesh
{
  struct VertexType
  {
    XMFLOAT3 position;
    XMFLOAT4 color; 
  };

  struct ModelType
  {
    float x, y, z;
    float tu, tv; 
  };

  virtual bool InitializeBuffers(ID3D11Device*);
  virtual void ShutdownBuffers();
  virtual void RenderBuffers(ID3D11DeviceContext*);
};




#endif
