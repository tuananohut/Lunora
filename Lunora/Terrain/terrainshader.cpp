#include "terrainshader.h"

TerrainShader::TerrainShader()
{
  m_vertexShader = nullptr;
  m_pixelShader = nullptr;
  m_layout = nullptr;
  m_matrixBuffer = nullptr;
  m_sampleState = nullptr;
  m_lightBuffer = nullptr; 
}

TerrainShader::TerrainShader(const TerrainShader& other) {}

TerrainShader::~TerrainShader() {}

bool TerrainShader::Initialize(ID3D11Device* device, HWND hwnd)
{
  bool result;

  result = InitializeShader(device, hwnd, L"../Lunora/Terrain/terrain.vs",
			    L"../Lunora/Terrain/terrain.ps");
  if (!result)
    {
      return false; 
    }

  return true; 
}

void TerrainShader::Shutdown()
{
  ShutdownShader(); 
}

bool TerrainShader::Render(ID3D11DeviceContext* deviceContext,
			   int indexCount,
			   XMMATRIX worldMatrix,
			   XMMATRIX viewMatrix,
			   XMMATRIX projectionMatrix,
			   ID3D11ShaderResourceView* texture,
			   XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
  bool result;

  result = SetShaderParameters(deviceContext,
			       worldMatrix, viewMatrix, projectionMatrix,
			       texture, lightDirection, diffuseColor);
  if (!result)
    {
      return false; 
    }

  RenderShader(deviceContext, indexCount);

  return true; 
}

bool TerrainShader::Initialize(ID3D11Device* device,
			       HWND hwnd,
			       WCHAR* vsFilename,
			       WCHAR* psFilename)
{
  HRESULT result;
  ID3D10Blob* errorMessage;
  ID3D10Blob* vertexShaderBuffer;
  ID3D10Blob* pixelShaderBuffer;
  D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
  unsigned int numElements;
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC lightBufferDesc;

  
}
