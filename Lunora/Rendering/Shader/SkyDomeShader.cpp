#include "SkyDomeShader.h"

HRESULT InitializeShaderResources(RendererContext& RenderBuffers, SkyDomeShader* shader)
{
  HRESULT hr; 
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;
  
  D3DCompileFromFile(L"c:/dev/Lunora/Shaders/skydome.hlsl", 0, 0, "SkyDomeVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
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
  
  D3DCompileFromFile(L"c:/dev/Lunora/Shaders/skydome.hlsl", 0, 0, "SkyDomePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader->baseShader.m_pixelShader);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
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
  
  D3D11_BUFFER_DESC colorBufferDesc;
  colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  colorBufferDesc.ByteWidth = sizeof(SkyDomeBufferType);
  colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  colorBufferDesc.MiscFlags = 0;
  colorBufferDesc.StructureByteStride = 0;

  hr = RenderBuffers.Device->CreateBuffer(&colorBufferDesc, NULL, &shader->m_colorBuffer);
  if(FAILED(hr))
    {
      return false;
    }
  
  return true;
}

bool Render(RendererContext& RenderBuffers, SkyDomeShader* shader,
	    uint32_t indexCount,
	    XMMATRIX world, XMMATRIX view, XMMATRIX proj,
	    XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
  HRESULT hr; 
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  SkyDomeBufferType* dataPtr2; 
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

  hr = RenderBuffers.DeviceContext->Map(shader->m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    return false;

  dataPtr2 = (SkyDomeBufferType*)mappedResource.pData;

  dataPtr2->apexColor = apexColor; 
  dataPtr2->centerColor = centerColor; 
  
  RenderBuffers.DeviceContext->Unmap(shader->m_colorBuffer, 0);

  bufferNumber = 0;

  RenderBuffers.DeviceContext->PSSetConstantBuffers(bufferNumber, 1, &shader->m_colorBuffer); 
  
  RenderBuffers.DeviceContext->IASetInputLayout(shader->baseShader.m_layout);
  		
  RenderBuffers.DeviceContext->VSSetShader(shader->baseShader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader->baseShader.m_pixelShader, NULL, 0);
  	
  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);
  
  return true;
}

void ReleaseShaderResources(SkyDomeShader* shader)
{
  if (shader)
    {
      shader->baseShader.Release();

      if (shader->m_colorBuffer)
	{
	  shader->m_colorBuffer->Release();
	  shader->m_colorBuffer = nullptr; 
	}
    }
}
