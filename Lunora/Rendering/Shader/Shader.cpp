#include "Shader.h"

HRESULT InitializeShaderResources(RendererContext& RenderBuffers,
				  ColorShader& shader)
{
  HRESULT hr; 
  D3D11_BUFFER_DESC matrixBufferDesc;
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;

  D3DCompileFromFile(L"../../Lunora/Shaders/gpu.hlsl", 0, 0, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3DCompileFromFile(L"../../Lunora/Shaders/gpu.hlsl", 0, 0, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader.m_pixelShader);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout)[0];
  
  hr = RenderBuffers.Device->CreateInputLayout(input_layout,
					       num_elements,
					       vsBlob->GetBufferPointer(),
					       vsBlob->GetBufferSize(),
					       &shader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0; 

  hr = RenderBuffers.Device->CreateBuffer(&matrixBufferDesc, NULL, &shader.m_matrixBuffer);
  if (FAILED(hr))
    {
      return false; 
    }
  
  return true; 
}

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader& shader)
{
  HRESULT hr; 
  D3D11_BUFFER_DESC matrixBufferDesc;
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;

  D3DCompileFromFile(L"../../Lunora/Shaders/texture.hlsl", 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3DCompileFromFile(L"../../Lunora/Shaders/texture.hlsl", 0, 0, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader.m_pixelShader);
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
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout)[0];
  
  hr = RenderBuffers.Device->CreateInputLayout(input_layout,
					       num_elements,
					       vsBlob->GetBufferPointer(),
					       vsBlob->GetBufferSize(),
					       &shader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0; 

  hr = RenderBuffers.Device->CreateBuffer(&matrixBufferDesc, NULL, &shader.m_matrixBuffer);
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

  hr = RenderBuffers.Device->CreateSamplerState(&samplerDesc, &shader.m_sampleState);
  if (FAILED(hr))
    {
      return false; 
    }
  
  return true;
}

bool Render(RendererContext& RenderBuffers,
	    ColorShader& shader,
	    UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
  HRESULT hr; 
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  unsigned int bufferNumber;
  
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  proj = XMMatrixTranspose(proj);
  
  hr = RenderBuffers.DeviceContext->Map(shader.m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(shader.m_matrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &shader.m_matrixBuffer); 

  RenderBuffers.DeviceContext->IASetInputLayout(shader.m_layout);
  
  RenderBuffers.DeviceContext->VSSetShader(shader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader.m_pixelShader, NULL, 0);
  
  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);

  return true; 
}

bool Render(RendererContext& RenderBuffers, TextureShader& shader,
	    UINT indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    ID3D11ShaderResourceView* texture)
{
  HRESULT hr; 
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  unsigned int bufferNumber;
  
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  proj = XMMatrixTranspose(proj);
  
  hr = RenderBuffers.DeviceContext->Map(shader.m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(shader.m_matrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &shader.m_matrixBuffer); 

  RenderBuffers.DeviceContext->PSSetShaderResources(0, 1, &texture);
  
  RenderBuffers.DeviceContext->IASetInputLayout(shader.m_layout);
  		
  RenderBuffers.DeviceContext->VSSetShader(shader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader.m_pixelShader, NULL, 0);
  	
  RenderBuffers.DeviceContext->PSSetSamplers(0, 1, &shader.m_sampleState);
  	
  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);
    
  return true;
}

void ReleaseShaderResources(ColorShader& shader)
{
  if (shader.m_matrixBuffer)
    {
      shader.m_matrixBuffer->Release();
      shader.m_matrixBuffer = nullptr; 
    }

  if (shader.m_vertexShader)
    {
      shader.m_vertexShader->Release();
      shader.m_vertexShader = nullptr;
    }

  if (shader.m_pixelShader)
    {
      shader.m_pixelShader->Release();
      shader.m_pixelShader = nullptr; 
    }

  if (shader.m_layout)
    {
      shader.m_layout->Release();
      shader.m_layout = nullptr;
    }
}

void ReleaseShaderResources(TextureShader& shader)
{
  if (shader.m_matrixBuffer)
    {
      shader.m_matrixBuffer->Release();
      shader.m_matrixBuffer = nullptr; 
    }

  if (shader.m_vertexShader)
    {
      shader.m_vertexShader->Release();
      shader.m_vertexShader = nullptr;
    }

  if (shader.m_pixelShader)
    {
      shader.m_pixelShader->Release();
      shader.m_pixelShader = nullptr; 
    }

  if (shader.m_layout)
    {
      shader.m_layout->Release();
      shader.m_layout = nullptr;
    }

  if (shader.m_sampleState)
    {
      shader.m_sampleState->Release();
      shader.m_sampleState = nullptr; 
    }
}
