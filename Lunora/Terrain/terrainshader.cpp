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

  result = D3DCompileFromFile(vsFilename, NULL, NULL, "TerrainVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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

  result = D3DCompileFromFile(psFilename, NULL, NULL, "TerrainPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
  if (FAILED(result))
    {
      if (errorMessage)
	{
	  OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
	}

      else
	{
	  MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
	}

      return false;
    }

  result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
  if (FAILED(result))
    {
      return false;
    }

  result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
  if (FAILED(result))
    {
      return false;
    }

  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  polygonLayout[1].SemanticName = "TEXCOORD";
  polygonLayout[1].SemanticIndex = 0;
  polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygonLayout[1].InputSlot = 0;
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "NORMAL";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "COLOR";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
  if (FAILED(result))
    {
      return false;
    }

  vertexShaderBuffer->Release();
  vertexShaderBuffer = nullptr;

  pixelShaderBuffer->Release();
  pixelShaderBuffer = nullptr;

  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
  if (FAILED(result))
    {
      return false;
    }

  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
  if (FAILED(result))
    {
      return false;
    }

  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBufferType);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
  if(FAILED(result))
    {
      return false;
    }
  
  return true;
}
