#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <string>

#include "Renderer.h"

struct HexMeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};

struct MeshVertexBuffer
{
  XMFLOAT3 position;
  XMFLOAT2 texture; 
  XMFLOAT3 normal;
  XMFLOAT3 tangent;
  XMFLOAT3 binormal; 
  XMFLOAT3 color; 
};

ID3D11Buffer* g_pVertexBuffer;

ID3D11VertexShader* m_vertexShader = nullptr;
ID3D11PixelShader* m_pixelShader = nullptr;

ID3D11Buffer* g_pMatrixBuffer;

struct MatrixBufferType
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

struct SimpleVertexCombined
{
  XMFLOAT3 Pos;
  XMFLOAT4 Col; 
};

SimpleVertexCombined verticesCombo[] =
{
  { XMFLOAT3(-1.f, -1.f, 0.f ), XMFLOAT4(1.f,0.f,0.f,1.f) }, 
  { XMFLOAT3( 0.f, 1.f, 0.f ), XMFLOAT4(0.f,1.f,0.f,1.f) }, 
  { XMFLOAT3( 1.f, -1.f, 0.f ), XMFLOAT4(0.f,0.f,1.f,1.f) }, 
};

HRESULT CreateVertexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(SimpleVertexCombined) * _countof(verticesCombo);
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = verticesCombo;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pVertexBuffer );
  
  return hr; 
}

ID3D11Buffer *g_pIndexBuffer = NULL;

unsigned int indices[] = { 0, 1, 2 };

HRESULT CreateIndexBuffer(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  
  D3D11_BUFFER_DESC bufferDesc;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(unsigned int) * _countof(indices);
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = indices;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;

  hr = RenderBuffers.Device->CreateBuffer( &bufferDesc, &InitData, &g_pIndexBuffer );
  
  return hr; 
}

void IAStage(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  ID3D11InputLayout* layout;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC matrixBufferDesc;
  ID3DBlob *vsBlob;
  ID3DBlob *psBlob;
  ID3DBlob *errorMessage;

  D3DCompileFromFile(L"../Lunora/gpu.hlsl", 0, 0, "vertex_shader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsBlob, &errorMessage);
  hr = RenderBuffers.Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &m_vertexShader);

  D3DCompileFromFile(L"../Lunora/gpu.hlsl", 0, 0, "pixel_shader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &psBlob, 0);
  RenderBuffers.Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &m_pixelShader);

  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0; 
  RenderBuffers.Device->CreateBuffer(&matrixBufferDesc, NULL, &g_pMatrixBuffer);
  
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
  
  RenderBuffers.DeviceContext->IASetInputLayout( layout );
  
  vsBlob->Release();
  psBlob->Release();
}

void Render(CoreRenderBuffers& RenderBuffers, Camera* Camera)
{
  UINT stride = sizeof( SimpleVertexCombined );
  UINT offset = 0;
  
  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 
						  1,
						  &g_pVertexBuffer,
						  &stride, 
						  &offset );

  RenderBuffers.DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  
  
  
  RenderBuffers.DeviceContext->RSSetViewports(1, &RenderBuffers.Viewport);
  
  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  unsigned int bufferNumber;
  XMMATRIX world = XMMatrixIdentity();
  XMMATRIX view;

  Camera->Render(); 
  
  Camera->GetViewMatrix(view);

  float fieldOfView = 3.141592654f / 4.0f;
  float screenAspect = (float)1080 / (float)720;

  const float SCREEN_DEPTH = 1000.f;
  const float SCREEN_NEAR = 0.1f;
  
  XMMATRIX proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
  
  world = XMMatrixTranspose(world);
  view = XMMatrixTranspose(view);
  proj = XMMatrixTranspose(proj);
  
  RenderBuffers.DeviceContext->Map(g_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

  dataPtr = (MatrixBufferType*)mappedResource.pData;

  dataPtr->world = world;
  dataPtr->view = view;
  dataPtr->proj = proj;

  RenderBuffers.DeviceContext->Unmap(g_pMatrixBuffer, 0);

  bufferNumber = 0;

  RenderBuffers.DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &g_pMatrixBuffer); 

 
  RenderBuffers.DeviceContext->OMSetRenderTargets(
						  1,
						  &RenderBuffers.RenderTargetView,
						  RenderBuffers.DepthStencilView
						  );

  RenderBuffers.DeviceContext->VSSetShader(m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(m_pixelShader, NULL, 0);
  
  
  RenderBuffers.DeviceContext->DrawIndexed(3, 0, 0);
}

