#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Shader
{
  struct MatrixBufferType
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection; 
  };

  virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
  virtual void ShutdownShader();
  virtual bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
  virtual void RenderShader(ID3D11DeviceContext*, int);

  ID3D11VertexShader* vertexShader;
  ID3D11PixelShader* pixelShader;
  ID3D11InputLayout* layout;
  ID3D11Buffer* matrixBuffer; 
};

#endif
