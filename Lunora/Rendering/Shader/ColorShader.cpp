#include "ColorShader.h"

HRESULT InitializeShaderResources(RendererContext& RenderBuffers,
				  ColorShader* shader)
{
  HRESULT hr;
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  ID3DBlob *errorMessage = nullptr;

  hr = CompileShaderFromFile(L"../../Lunora/Shaders/color.hlsl",
			     "ColorVertexShader",
			     "vs_5_0",
			     &vsBlob);
  if (FAILED(hr))
    {
      return false; 
    }
  
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &shader->baseShader.m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }

  hr = CompileShaderFromFile(L"../../Lunora/Shaders/color.hlsl",
			     "ColorPixelShader",
			     "ps_5_0",
			     &psBlob);
  if (FAILED(hr))
    {
      return false; 
    }

  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &shader->baseShader.m_pixelShader);
  if (FAILED(hr))
    {
      return false; 
    }

  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout)[0];
  
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
  
  return true; 
}

bool Render(RendererContext& RenderBuffers,
	    ColorShader* shader,
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

  RenderBuffers.DeviceContext->IASetInputLayout(shader->baseShader.m_layout);
  
  RenderBuffers.DeviceContext->VSSetShader(shader->baseShader.m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(shader->baseShader.m_pixelShader, NULL, 0);
  
  RenderBuffers.DeviceContext->DrawIndexed(indexCount, 0, 0);

  return true; 
}


void ReleaseShaderResources(ColorShader* shader)
{
  if (shader)
    {
      shader->baseShader.Release();
    }
}
