#include <cstdio>

#include "ShaderSystem.h"

static bool CompileShader(const wchar_t* filename,
			  const char* entryPoint,
			  const char* target,
			  ID3DBlob** blobOut)
{
  ID3DBlob* errorBlob = nullptr;
  HRESULT result = D3DCompileFromFile(filename,
				      nullptr,
				      nullptr,
				      entryPoint,
				      target,
				      D3DCOMPILE_ENABLE_STRICTNESS,
				      0,
				      blobOut,
				      &errorBlob);
  if (FAILED(result))
    {
      if (errorBlob)
	{
	  OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	  errorBlob->Release();
	}
      else
	{
	  OutputDebugStringA("Shader error: Unknown.\n");
	}

      return false; 
    }

  return true;
}

bool LoadShader(ID3D11Device* Device,
		const wchar_t* vsPath,
		const wchar_t* psPath,
		ShaderGPUData* Shader,
		RenderManager* Renderer)
{  
  ID3DBlob* vsBlob = nullptr;
  ID3DBlob* psBlob = nullptr;

  if (!CompileShader(vsPath, "LightVertexShader", "vs_5_0", &vsBlob))
    {
      return false; 
    }

  if (!CompileShader(psPath, "GoochPixelShader", "ps_5_0", &psBlob))
    {
      return false;
    }

  HRESULT result = Device->CreateVertexShader(vsBlob->GetBufferPointer(),
					      vsBlob->GetBufferSize(),
					      nullptr,
					      &Shader->VertexShader);
  if (FAILED(result))
    {
      return false; 
    }

  result = Device->CreatePixelShader(psBlob->GetBufferPointer(),
				     psBlob->GetBufferSize(),
				     nullptr,
				     &Shader->PixelShader);
  if (FAILED(result))
    {
      return false; 
    }
  
  D3D11_INPUT_ELEMENT_DESC* layout = Renderer->GetLayout(ShaderLayoutType::Light);
  
  result = Device->CreateInputLayout(layout,
				     2,
				     vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
				     &Shader->InputLayout);

  vsBlob->Release();
  psBlob->Release();

  return SUCCEEDED(result);
}

void UseShader(ID3D11DeviceContext* DeviceContext,
	       const ShaderGPUData* Shader)
{ 
  DeviceContext->IASetInputLayout(Shader->InputLayout);
  DeviceContext->VSSetShader(Shader->VertexShader, nullptr, 0);
  DeviceContext->PSSetShader(Shader->PixelShader, nullptr, 0);
}

void ReleaseShader(ShaderGPUData* Shader)
{  
  if (Shader->VertexShader)
    {
      Shader->VertexShader->Release();
      Shader->VertexShader = nullptr; 
    }

  if (Shader->PixelShader)
    {
      Shader->PixelShader->Release();
      Shader->PixelShader = nullptr;
    }

  if (Shader->InputLayout)
    {
      Shader->InputLayout->Release();
      Shader->InputLayout = nullptr; 
    }
}
