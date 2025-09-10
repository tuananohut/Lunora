#include <DirectXMath.h>
#include <d3dcompiler.h>
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

struct SimpleVertexCombined
{
  XMFLOAT3 Pos;
  XMFLOAT3 Col; 
};

SimpleVertexCombined verticesCombo[] =
  {
    XMFLOAT3(  0.f ,  0.5f,  0.5f  ),
    XMFLOAT3(  0.f ,  0.f ,  0.5f  ),
    XMFLOAT3(  0.5f, -0.5f,  0.5f  ),
    XMFLOAT3(  0.5f, -0.5f,  0.5f  ),
    XMFLOAT3( -0.5f, -0.5f,  0.5f  ),
    XMFLOAT3(  0.f,   0.5f,  0.f   ),
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
  ID3D10Blob* errorMessage = nullptr;
  ID3D10Blob* vertexShaderBuffer = nullptr;
  ID3D10Blob* pixelShaderBuffer = nullptr;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC matrixBufferDesc;

  result = D3DCompileFromFile("simple_vertex_shader.vs", NULL, NULL,
			      "VSmain", "vs_5_0",
			      D3D10_SHADER_ENABLE_STRICTNESS, 0,
			      &vertexShaderBuffer, &errorMessage);
  if (FAILED(result))
    {
      if (errorMessage)
	{
	  OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
	}
      else
	{
	  MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK | MB_ICONERROR);
	}

      return false;
    }
  
  D3D11_INPUT_ELEMENT_DESC input_layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

  UINT num_elements = sizeof(layout) / sizeof(layout)[0];
  
  hr = RenderBuffers.Device->CreateInputLayout(&input_layout, num_elements, vertexShaderBuffer->, layout);
  
  UINT stride = sizeof( SimpleVertex );
  UINT offset = 0;
  RenderBuffers.Device->IASetVertexBuffers(
					   0, 
					   1, 
					   &g_pVertexBuffer,
					   &stride, 
					   &offset );
  
  RenderBuffers.Device->IASetInputLayout( g_pVertexLayout );
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
}
