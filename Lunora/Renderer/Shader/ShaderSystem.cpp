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

static int FindShaderIndex(const ShaderData* data, UINT entityID)
{
  for (UINT i = 0; i < data->Count; ++i)
    {
      if (data->EntityIDs[i] == entityID)
	{
	  return i; 
	}
    }

  return -1;
}

bool LoadShader(ID3D11Device* Device,
		const wchar_t* vsPath,
		const wchar_t* psPath,
		ShaderData* shaderData,
		RenderManager* Renderer,
		UINT entityID)
{
  if (shaderData->Count >= shaderData->Capacity)
    {
      return false; 
    }
  
  ID3DBlob* vsBlob = nullptr;
  ID3DBlob* psBlob = nullptr;

  if (!CompileShader(vsPath, "LightVertexShader", "vs_5_0", &vsBlob))
    {
      return false; 
    }

  if (!CompileShader(psPath, "LightPixelShader", "ps_5_0", &psBlob))
    {
      return false;
    }

  UINT index = shaderData->Count++;

  ShaderGPUData* gpuShader = &shaderData->ShaderArray[index];
  shaderData->EntityIDs[index] = entityID;
  
  HRESULT result = Device->CreateVertexShader(vsBlob->GetBufferPointer(),
					      vsBlob->GetBufferSize(),
					      nullptr,
					      &gpuShader->VertexShader);
  if (FAILED(result))
    {
      return false; 
    }

  result = Device->CreatePixelShader(psBlob->GetBufferPointer(),
				     psBlob->GetBufferSize(),
				     nullptr,
				     &gpuShader->PixelShader);
  if (FAILED(result))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC PolygonLayout[2] = { Renderer->LightShader()[0], 
						Renderer->LightShader()[1]}; 
  
  result = Device->CreateInputLayout(PolygonLayout,
				     2,
				     vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
				     &gpuShader->InputLayout);

  vsBlob->Release();
  psBlob->Release();

  return SUCCEEDED(result);
}

void UseShader(ID3D11DeviceContext* DeviceContext,
	       const ShaderData* ShaderData,
	       UINT entityID)
{
  int index = FindShaderIndex(ShaderData, entityID);
  if (index == -1)
    return; 
  
  const ShaderGPUData* Shader = &ShaderData->ShaderArray[index];
  
  DeviceContext->IASetInputLayout(Shader->InputLayout);
  DeviceContext->VSSetShader(Shader->VertexShader, nullptr, 0);
  DeviceContext->PSSetShader(Shader->PixelShader, nullptr, 0);
}

void ReleaseShader(ShaderData* ShaderData, UINT entityID)
{
  int index = FindShaderIndex(ShaderData, entityID);
  if (index == -1)
    return; 

  ShaderGPUData* Shader = &ShaderData->ShaderArray[index]; 
  
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

  ShaderData->Count--;
  ShaderData->ShaderArray[index] = ShaderData->ShaderArray[ShaderData->Count];
  ShaderData->EntityIDs[index] = ShaderData->EntityIDs[ShaderData->Count];
}

void ReleaseAllShaders(ShaderData* ShaderData)
{
  for (UINT i = 0; i < ShaderData->Count; ++i)
    {
      ShaderGPUData* Shader = &ShaderData->ShaderArray[i];

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

  ShaderData->Count = 0; 
}
