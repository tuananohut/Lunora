#ifndef COLORSHADER_H
#define COLORSHADER_H

#include "Shader.h"

struct ColorShader: public Shader
{
  struct TessellationBufferType
  {
    float tessellationAmount;
    XMFLOAT3 padding; 
  };

  ColorShader();
  ~ColorShader();
  
  bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
  void ShutdownShader();
  bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
  void RenderShader(ID3D11DeviceContext*, int);

  ID3D11VertexShader* vertexShader;
  ID3D11PixelShader* pixelShader;
  ID3D11InputLayout* layout;
  ID3D11Buffer* matrixBuffer; 
};

#endif
