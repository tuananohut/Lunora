#include "Shader.h"

HRESULT CompileShaderFromFile(WCHAR* filename,
			      const char* entryPoint,
			      const char* shaderModel,
			      ID3DBlob** outBlob)
{
  *outBlob = nullptr;
  ID3DBlob* errorMessage = nullptr;

  HRESULT hr = D3DCompileFromFile(filename, 0, 0, entryPoint, shaderModel,
				  D3D10_SHADER_ENABLE_STRICTNESS, 0,
				  outBlob, &errorMessage);
  if (FAILED(hr))
    {
      if (errorMessage)
	{
	  MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(),
		      "Shader compilation error.", MB_OK | MB_ICONERROR);
	  errorMessage->Release();
	}
      else
	{
	  MessageBoxW(NULL, filename, L"Could not shader file", MB_OK | MB_ICONERROR);
	}
      
      return hr; 
    }

  if (errorMessage)
    {
      errorMessage->Release(); 
    }

  return S_OK;
}

void BaseShader::Release()
{
  if (m_matrixBuffer)
    {
      m_matrixBuffer->Release();
      m_matrixBuffer = nullptr; 
    }

  if (m_vertexShader)
    {
      m_vertexShader->Release();
      m_vertexShader = nullptr;
    }
  
  if (m_pixelShader)
    {
      m_pixelShader->Release();
      m_pixelShader = nullptr; 
    }

  if (m_layout)
    {
      m_layout->Release();
      m_layout = nullptr;
    }
}

HRESULT CreateMatrixBuffer(ID3D11Device* device, ID3D11Buffer** matrixBuffer)
{
  D3D11_BUFFER_DESC matrixBufferDesc;
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0; 
  
  return device->CreateBuffer(&matrixBufferDesc, NULL, matrixBuffer);
}

