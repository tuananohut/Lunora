#include "AmbientLightShader.h"

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, AmbientLightShader* shader)
{
  HRESULT hr; 
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;
  
  D3DCompileFromFile(L"c:/dev/Lunora/Shaders/ambient_light.hlsl", 0, 0, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader->baseShader.m_vertexShader);
  if (FAILED(hr))
    {
      if (errorMessage)
	{
	  OutputDebugStringA((char*)errorMessage->GetBufferPointer());
	  errorMessage->Release();
	}
      assert(false);
      return false;
    }
  
  D3DCompileFromFile(L"c:/dev/Lunora/Shaders/ambient_light.hlsl", 0, 0, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader->baseShader.m_pixelShader);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 } 
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout[0]);
  
  hr = RenderBuffers.Device->CreateInputLayout(input_layout,
					       num_elements,
					       vsBlob->GetBufferPointer(),
					       vsBlob->GetBufferSize(),
					       &shader->baseShader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  hr = CreateMatrixBuffer(RenderBuffers.Device, &shader->baseShader.m_matrixBuffer);
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
  lightBufferDesc.ByteWidth = sizeof(AmbientLightBufferType);
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

bool Render(RendererContext& RenderBuffers, AmbientLightShader* shader,
	    uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture,
	    XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection)
{
  HRESULT hr; 
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  AmbientLightBufferType* dataPtr2; 
  unsigned int bufferNumber;
  
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  proj = XMMatrixTranspose(proj);
  
  hr = RenderBuffers.DeviceContext->Map(shader->baseShader.m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(shader->baseShader.m_matrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &shader->baseShader.m_matrixBuffer); 

  RenderBuffers.DeviceContext->PSSetShaderResources(0, 1, &texture);

  hr = RenderBuffers.DeviceContext->Map(shader->m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    return false;

  dataPtr2 = (AmbientLightBufferType*)mappedResource.pData;

  dataPtr2->ambientColor = ambientColor;
  dataPtr2->diffuseColor = diffuseColor;
  dataPtr2->lightDirection = lightDirection;
  dataPtr2->padding = 0.0f;
  
  RenderBuffers.DeviceContext->Unmap(shader->m_lightBuffer, 0);

  bufferNumber = 0;

  RenderBuffers.DeviceContext->PSSetConstantBuffers(bufferNumber, 1, &shader->m_lightBuffer); 
  
  RenderBuffers.DeviceContext->IASetInputLayout(shader->baseShader.m_layout);
  		
  RenderBuffers.DeviceContext->VSSetShader(shader->baseShader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader->baseShader.m_pixelShader, NULL, 0);
  	
  RenderBuffers.DeviceContext->PSSetSamplers(0, 1, &shader->m_sampleState);

  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);
  
  return true;
}

void ReleaseShaderResources(AmbientLightShader* shader)
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
