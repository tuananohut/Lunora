#include "skydomeshader.h"

SkyDomeShader::SkyDomeShader()
{
  m_vertexShader = nullptr;
  m_pixelShader = nullptr;
  m_layout = nullptr;
  m_matrixBuffer = nullptr;
  m_colorBuffer = nullptr; 
}

SkyDomeShader::SkyDomeShader(const SkyDomeShader& other) {}

SkyDomeShader::~SkyDomeShader() {}

bool SkyDomeShader::Initialize(ID3D11Device* device, HWND hwnd)
{
  bool result;

  result = InitializeShader(device, hwnd,
			    L"../Lunora/Terrain/skydome.vs",
			    L"../Lunora/Terrain/skydome.ps");
  if (!result)
    {
      return false;
    }

  return true; 
}

void SkyDomeShader::Shutdown()
{
  ShutdownShader();
}

bool SkyDomeShader::Render(ID3D11DeviceContext* deviceContext,
			   int indexCount,
			   XMMATRIX worldMatrix,
			   XMMATRIX viewMatrix,
			   XMMATRIX projectionMatrix,
			   XMFLOAT4 apexColor,
			   XMFLOAT4 centerColor)
{
  bool result;

  result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
  if (!result)
    {
      return false;
    }

  RenderShader(deviceContext, indexCount);

  return true; 
}

bool SkyDomeShader::InitializeShader(ID3D11Device* device,
				     HWND hwnd,
				     WCHAR* vsFilename,
				     WCHAR* psFilename)
{
  HRESULT result;
  ID3D10Blob* errorMessage;
  ID3D10Blob* vertexShaderBuffer;
  ID3D10Blob* pixelShaderBuffer;
  D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
  unsigned int numElements;
  D3D11_BUFFER_DESC matrixBufferDesc;
  D3D11_BUFFER_DESC colorBufferDesc;

  errorMessage = nullptr;
  vertexShaderBuffer = nullptr;
  pixelShaderBuffer = nullptr;

  result = D3DCompileFromFile(vsFilename, NULL, NULL, "SkyDomeVertexShader", "vs_5_0",
			      D3D_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer,
			      &errorMessage);
  if (FAILED(result))
    {
      if (errorMessage)
	OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
      else
	MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);

      return false;
    }

  result = D3DCompileFromFile(psFilename, NULL, NULL, "SkyDomePixelShader", "ps_5_0",
			      D3D_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer,
			      &errorMessage);
  if (FAILED(result))
    {
      if (errorMessage)
	OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
      else
	MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);

      return false;
    }

  result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
				      vertexShaderBuffer->GetBufferSize(),
				      NULL, &m_vertexShader);
  if (FAILED(result))
    return false;
  
  result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
				     pixelShaderBuffer->GetBufferSize(),
				     NULL, &m_pixelShader);
  if (FAILED(result))
    return false;

  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  result = device->CreateInputLayout(polygonLayout, numElements,
				     vertexShaderBuffer->GetBufferPointer(),
				     vertexShaderBuffer->GetBufferSize(),
				     &m_layout);
  if (FAILED(result))
    return false;

  vertexShaderBuffer->Release();
  vertexShaderBuffer = nullptr;

  pixelShaderBuffer->Release();
  pixelShaderBuffer = nullptr; 
  
}
