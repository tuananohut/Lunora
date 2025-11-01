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

HRESULT InitializeShaderResources(RendererContext& RenderBuffers,
				  ColorShader& shader)
{
  HRESULT hr;
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;

  hr = CompileShaderFromFile(L"../../Lunora/Shaders/gpu.hlsl",
			     "ColorVertexShader",
			     "vs_5_0",
			     &vsBlob);
  if (FAILED(hr))
    {
      return false; 
    }
  
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader.baseShader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }

  hr = CompileShaderFromFile(L"../../Lunora/Shaders/gpu.hlsl",
			     "ColorPixelShader",
			     "ps_5_0",
			     &psBlob);
  if (FAILED(hr))
    {
      return false; 
    }

  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader.baseShader.m_pixelShader);
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
					       &shader.baseShader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  hr = CreateMatrixBuffer(RenderBuffers.Device, &shader.baseShader.m_matrixBuffer);
  if (FAILED(hr))
    {
      return false; 
    }
  
  return true; 
}

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, TextureShader& shader)
{
  HRESULT hr; 
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;
  
  D3DCompileFromFile(L"../../Lunora/Shaders/texture.hlsl", 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader.baseShader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3DCompileFromFile(L"../../Lunora/Shaders/texture.hlsl", 0, 0, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader.baseShader.m_pixelShader);
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
					       &shader.baseShader.m_layout);
  if (FAILED(hr))
    {
      return false; 
    }
  
  vsBlob->Release();
  vsBlob = nullptr; 

  psBlob->Release();
  psBlob = nullptr;
  
  hr = CreateMatrixBuffer(RenderBuffers.Device, &shader.baseShader.m_matrixBuffer);
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
  
  hr = RenderBuffers.DeviceContext->Map(shader.baseShader.m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(shader.baseShader.m_matrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &shader.baseShader.m_matrixBuffer); 

  RenderBuffers.DeviceContext->IASetInputLayout(shader.baseShader.m_layout);
  
  RenderBuffers.DeviceContext->VSSetShader(shader.baseShader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader.baseShader.m_pixelShader, NULL, 0);
  
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
  
  hr = RenderBuffers.DeviceContext->Map(shader.baseShader.m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(shader.baseShader.m_matrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &shader.baseShader.m_matrixBuffer); 

  RenderBuffers.DeviceContext->PSSetShaderResources(0, 1, &texture);
  
  RenderBuffers.DeviceContext->IASetInputLayout(shader.baseShader.m_layout);
  		
  RenderBuffers.DeviceContext->VSSetShader(shader.baseShader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader.baseShader.m_pixelShader, NULL, 0);
  	
  RenderBuffers.DeviceContext->PSSetSamplers(0, 1, &shader.m_sampleState);
  	
  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);
    
  return true;
}

void ReleaseShaderResources(ColorShader& shader)
{
  shader.baseShader.Release();
}

void ReleaseShaderResources(TextureShader& shader)
{
  shader.baseShader.Release();
  
  if (shader.m_sampleState)
    {
      shader.m_sampleState->Release();
      shader.m_sampleState = nullptr; 
    }
}
