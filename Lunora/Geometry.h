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


struct SimpleVertexCombined
{
  XMFLOAT3 Pos;
  XMFLOAT4 Col; 
};

SimpleVertexCombined verticesCombo[] =
{
  { XMFLOAT3(  0.0f,  0.5f, 0.5f ), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.f) },
  { XMFLOAT3(  0.5f, -0.5f, 0.5f ), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.f) },
  { XMFLOAT3( -0.5f, -0.5f, 0.5f ), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.f) },
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


HRESULT CompileShader(_In_ LPCWSTR srcFile,
		      _In_ LPCSTR entryPoint,
		      _In_ LPCSTR profile,
		      _Outptr_ ID3DBlob** blob)
{
  if (!srcFile || !entryPoint || !profile || !blob)
    {
      return E_INVALIDARG; 
    }

  *blob = nullptr;

  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined ( DEBUG ) || defined ( _DEBUG )
  flags |= D3DCOMPILE_DEBUG;
#endif

  const D3D_SHADER_MACRO defines[] =
  {
    "EXAMPLE_DEFINE", "1",
      NULL, NULL
      };

  ID3DBlob *shaderBlob = nullptr; 
  ID3DBlob *errorBlob = nullptr;
  HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				  entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
  if (FAILED(hr))
    {
      if (errorBlob)
	{
	  std::string msg((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
	  MessageBoxA(nullptr, msg.c_str(), "Shader Compile Error", MB_OK);
	  errorBlob->Release();
	}
      return hr;
    }
 

  *blob = shaderBlob; 

  return hr; 
}

void IAStage(CoreRenderBuffers& RenderBuffers)
{
  HRESULT hr; 
  ID3D11InputLayout* layout;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC matrixBufferDesc;
  ID3DBlob *vsBlob = nullptr;
  ID3DBlob *psBlob = nullptr;
  
  hr = CompileShader(L"simple_vertex_shader.vs", "VSMain", "vs_4_0_level_9_1", &vsBlob);


  hr = CompileShader(L"simple_pixel_shader.vs", "PSMain", "ps_4_0_level_9_1", &psBlob);


  /*
  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

  UINT num_elements = sizeof(input_layout) / sizeof(input_layout)[0];
  
  hr = RenderBuffers.Device->CreateInputLayout(&input_layout,
					       num_elements,
					       vsBlob->GetBufferPointer(),
					       vsBlob->GetBufferSize(),
					       &layout);
  
  UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  /*
  RenderBuffers.Device->IASetVertexBuffers(0, 
					   1, 
					   &g_pVertexBuffer,
					   &stride, 
					   &offset );
  */

  
  RenderBuffers.Device->CreateVertexShader(
					   vsBlob->GetBufferPointer(),
					   vsBlob->GetBufferSize(),
					   nullptr,
					   &m_vertexShader
					   );

  RenderBuffers.Device->CreatePixelShader(
					  psBlob->GetBufferPointer(),
					  psBlob->GetBufferSize(),
					  nullptr,
					  &m_pixelShader
					  );

  
  RenderBuffers.DeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
  RenderBuffers.DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

  
  vsBlob->Release();
  psBlob->Release();
}

void Render(CoreRenderBuffers& RenderBuffers)
{
  // RenderBuffers.DeviceContext->IASetInputLayout( g_pVertexLayout );
  RenderBuffers.DeviceContext->IASetInputLayout(nullptr);

  RenderBuffers.DeviceContext->VSSetShader(m_vertexShader, NULL, 0);
  RenderBuffers.DeviceContext->PSSetShader(m_pixelShader, NULL, 0);

  RenderBuffers.DeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  RenderBuffers.DeviceContext->Draw(3, 0);
}

