#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "../../Engine/src/Renderer.h"

using namespace DirectX; 
using namespace LunoraEngine; 

struct BaseShader
{
  ID3D11Buffer *m_matrixBuffer = nullptr;
  ID3D11VertexShader *m_vertexShader = nullptr;
  ID3D11PixelShader *m_pixelShader = nullptr;
  ID3D11InputLayout *m_layout = nullptr;

  void Release(); 
};
   
struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

HRESULT CompileShaderFromFile(WCHAR* filename,
			      const char* entryPoint,
			      const char* shaderModel,
			      ID3DBlob** outBlob);

HRESULT CreateMatrixBuffer(ID3D11Device* device, ID3D11Buffer** matrixBuffer);

#endif
 
