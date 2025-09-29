#ifndef SHADER_H
#define SHADER_H

static ID3D11Buffer* g_pMatrixBuffer;

static ID3D11VertexShader* m_vertexShader = nullptr;
static ID3D11PixelShader* m_pixelShader = nullptr;

static ID3D11InputLayout* layout = NULL;

struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

struct SimpleVertexCombined
{
  XMFLOAT3 position;
  XMFLOAT4 color; 
};

HRESULT InitializeShaderResources(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC matrixBufferDesc;
  ID3DBlob *vsBlob;
  ID3DBlob *psBlob;
  ID3DBlob *errorMessage;

  D3DCompileFromFile(L"../Lunora/gpu.hlsl", 0, 0, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &m_vertexShader);
  if (FAILED(hr))
    {
      return false; 
    }
  
  D3DCompileFromFile(L"../Lunora/gpu.hlsl", 0, 0, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  hr = RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &m_pixelShader);
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
					       &layout);
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

  hr = RenderBuffers.Device->CreateBuffer(&matrixBufferDesc, NULL, &g_pMatrixBuffer);
  if (FAILED(hr))
    {
      return false; 
    }
  
  return true; 
}

bool Render(CoreRenderBuffers& RenderBuffers, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
  HRESULT hr; 
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  unsigned int bufferNumber;
  
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  proj = XMMatrixTranspose(proj);
  
  hr = RenderBuffers.DeviceContext->Map(g_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(hr))
    {
      return false;
    }
  
  dataPtr = (MatrixBufferType*)mappedResource.pData;
  
  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(g_pMatrixBuffer, 0);
  
  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &g_pMatrixBuffer); 

  RenderBuffers.DeviceContext->IASetInputLayout(layout);
  
  RenderBuffers.DeviceContext->VSSetShader(m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(m_pixelShader, NULL, 0);
  
  RenderBuffers.DeviceContext->DrawIndexed(3, 0, 0);

  return true; 
}



#endif
