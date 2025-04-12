#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include <d3d11.h>

struct ShaderData
{  
  ID3D11VertexShader* VertexShader;
  ID3D11PixelShader* PixelShader;
  ID3D11Buffer* LightBuffer;
  ID3D11InputLayout* InputLayout;   
};

struct MeshData
{
  ID3D11Buffer* MatrixBuffer;
  ID3D11Buffer *VertexBuffer; 
  ID3D11Buffer *IndexBuffer;
  UINT indexCount;
  UINT stride; 
};

struct CameraData {};

struct InputKeyData
{
  bool W;
  bool A;
  bool S;
  bool D;
};

#endif
