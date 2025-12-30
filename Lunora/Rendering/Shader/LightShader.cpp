#include "Shader.h"

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, LightShader* shader)
{
  HRESULT hr; 
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;
  
  D3DCompileFromFile(L"../../Lunora/Shaders/light.hlsl", 0, 0, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader->textureShader.baseShader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3DCompileFromFile(L"../../Lunora/Shaders/light.hlsl", 0, 0, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader->textureShader.baseShader.m_pixelShader);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 } 
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout)[0];
  
  hr = RenderBuffers.Device->CreateInputLayout(input_layout,
					       num_elements,
					       vsBlob->GetBufferPointer(),
					       vsBlob->GetBufferSize(),
					       &shader->textureShader.baseShader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  hr = CreateMatrixBuffer(RenderBuffers.Device, &shader->textureShader.baseShader.m_matrixBuffer);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_SAMPLER_DESC samplerDesc;
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; 

  hr = RenderBuffers.Device->CreateSamplerState(&samplerDesc, &shader->m_sampleState);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3D11_BUFFER_DESC lightBufferDesc;
  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBufferType);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  hr = RenderBuffers.Device->CreateBuffer(&lightBufferDesc, NULL, &shader->m_lightBuffer);
  if(FAILED(hr))
    {
      return false;
    }
  
  return true;
}

void ReleaseShaderResources(LightShader* shader)
{
  if (shader)
    {
      shader->baseShader.Release();
  
      if (shader->m_sampleState)
	{
	  shader->m_sampleState->Release();
	  shader->m_sampleState = nullptr; 
	}

      if (shader->m_lightBuffer)
	{
	  shader->m_lightBuffer->Release();
	  shader->m_lightBuffer = nullptr; 
	}

    }
}
