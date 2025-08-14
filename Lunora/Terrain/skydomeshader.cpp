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

  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
  if (FAILED(result))
    return false;

  colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  colorBufferDesc.ByteWidth = sizeof(ColorBufferType);
  colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  colorBufferDesc.MiscFlags = 0;
  colorBufferDesc.StructureByteStride = 0;

  result = device->CreateBuffer(&colorBufferDesc, NULL, &m_colorBuffer);
  if (FAILED(result))
    return false;

  return true; 
}

void SkyDomeShader::ShutdownShader()
{
  if (m_colorBuffer)
    {
      m_colorBuffer->Release();
      m_colorBuffer = nullptr;
    }

  if (m_matrixBuffer)
    {
      m_matrixBuffer->Release();
      m_matrixBuffer = nullptr;
    }

  if (m_layout)
    {
      m_layout->Release();
      m_layout = nullptr;
    }

  if (m_pixelShader)
    {
      m_pixelShader->Release();
      m_pixelShader = nullptr;
    }

  if (m_vertexShader)
    {
      m_vertexShader->Release();
      m_vertexShader = nullptr; 
    }
}

void SkyDomeShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage,
					     HWND hwnd,
					     WCHAR* shaderFilename)
{
  char* compileErrors;
  unsigned __int64 bufferSize, i;
  ofstream fout;

  compileErrors = (char*)(errorMessage->GetBufferPointer());

  bufferSize = errorMessage->GetBufferSize();

  fout.open("shader-error.txt");

  for (i = 0; i < bufferSize; i++)
    fout << compileErrors[i];

  fout.close();

  errorMessage->Release();
  errorMessage = nullptr;

  MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool SkyDomeShader::SetShaderParameters(ID3D11DeviceContext* deviceContext,
					XMMATRIX worldMatrix,
					XMMATRIX viewMatrix,
					XMMATRIX projectionMatrix,
					XMFLOAT4 apexColor,
					XMFLOAT4 centerColor)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType *dataPtr;
  unsigned int bufferNumber;
  ColorBufferType *dataPtr2;

  worldMatrix = XMMatrixTranspose(worldMatrix);
  viewMatirx = XMMatrixTranspose(viewMatrix);
  projectionMatrix = XMMatrixTranspose(projectionMatrix);

  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
    {
      return false;
    }

  dataPtr = (MatrixBufferType*)mappedResource.pData;

  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  deviceContext->Unmap(m_matrixBuffer, 0);

  bufferNumber = 0;

  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

  result = deviceContext->Map(m_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
    {
      return false; 
    }

  dataPtr2 = (ColorBufferType*)mappedResource.pData;

  dataPtr2->apexColor = apexColor;
  dataPtr2->centerColor = centerColor;

  deviceContext->Unmap(m_colorBuffer, 0);

  bufferNumber = 0;

  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_colorBuffer);

  return true;
}

void SkyDomeShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
  deviceContext->IASetInputLayout(m_layout);

  deviceContext->VSSetShader(m_vertexShader, NULL, 0);
  deviceContext->PSSetShader(m_pixelShader, NULL, 0);

  deviceContext->DrawIndexed(indexCount, 0, 0);
}
