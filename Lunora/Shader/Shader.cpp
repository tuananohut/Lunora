#include "Shader.h"

bool Shader::InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{
  return true;
}

void Shader::ShutdownShader() {}

bool Shader::SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX)
{
  return true;
}

void Shader::RenderShader(ID3D11DeviceContext*, int) {}
