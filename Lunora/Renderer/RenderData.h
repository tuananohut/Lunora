#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include <d3d11.h>

struct ShaderGPUData
{
  ID3D11VertexShader* VertexShader;
  ID3D11PixelShader* PixelShader;
  ID3D11InputLayout* InputLayout;
  ID3D11Buffer* LightBuffer;
};

struct MeshGPUData
{
  ID3D11Buffer* MatrixBuffer;
  ID3D11Buffer *VertexBuffer; 
  ID3D11Buffer *IndexBuffer;
  int indexCount;
  UINT stride; 
};

struct CameraData {};

enum InputKey
  {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_COUNT
  };

struct InputKeyData
{
  bool Keys[KEY_COUNT]; 
};

#endif
