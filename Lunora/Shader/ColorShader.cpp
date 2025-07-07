#include "ColorShader.h"

ColorShader::ColorShader()
{
  vertexShader = nullptr;
  pixelShader = nullptr;
  layout = nullptr;
  matrixBuffer = nullptr; 
}

ColorShader::~ColorShader() {}

bool ColorShader::InitializeShader(ID3D11Device* Device,
				   HWND hwnd,
				   WCHAR* vsFilename,
				   WCHAR* psFilename)
{
  HRESULT result;

  ID3D10Blob* vertexShaderBuffer = nullptr;
  ID3D10Blob* pixelShaderBuffer = nullptr;

  D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
  unsigned int numElements;
  D3D11_BUFFER_DESC matrixBufferDesc; 
  
  result = D3DCompileFromFile(vsFilename,
			      NULL, NULL,
			      "ColorVertexShader",
			      "vs_5_0",
			      D3D10_SHADER_ENABLE_STRICTNESS,
			      0,
			      &vertexShaderBuffer, NULL);
  if (FAILED(result))
    {
      MessageBoxW(hwnd, vsFilename, L"Vertex Shader File Error", MB_OK | MB_ICONERROR);
      return false;
    }

  result = D3DCompileFromFile(psFilename,
			      NULL, NULL,
			      "ColorPixelShader",
			      "ps_5_0",
			      D3D10_SHADER_ENABLE_STRICTNESS,
			      0,
			      &pixelShaderBuffer, NULL);
  if (FAILED(result))
    {
      MessageBoxW(hwnd, psFilename, L"Pixel Shader File Error", MB_OK | MB_ICONERROR);
      return false;
    }

  result = Device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
				      vertexShaderBuffer->GetBufferSize(),
				      NULL, &vertexShader);
  if (FAILED(result))
    {
      return false;
    }

  
  result = Device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
				     pixelShaderBuffer->GetBufferSize(),
				     NULL, &pixelShader);
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

  polygonLayout[1].SemanticName = "COLOR";
  polygonLayout[1].SemanticIndex = 0;      
  polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  
  polygonLayout[1].InputSlot = 0;          
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;   
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  result = Device->CreateInputLayout(polygonLayout,
				     numElements,
				     vertexShaderBuffer->GetBufferPointer(),
				     vertexShaderBuffer->GetBufferSize(),
				     &layout);
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

  result = Device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
  if (FAILED(result))
    {
      return false;
    }
  
  return true;
} 
 
void ColorShader::ShutdownShader()
{
  if (matrixBuffer)
    {
      matrixBuffer->Release();
      matrixBuffer = nullptr;
    }

  if (layout)
    {
      layout->Release();
      layout = nullptr;
    }

  if (pixelShader)
    {
      pixelShader->Release();
      pixelShader = nullptr;
    }

  if (vertexShader)
    {
      vertexShader->Release();
      vertexShader = nullptr;
    }
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext,
				      XMMATRIX worldMatrix,
				      XMMATRIX viewMatrix,
				      XMMATRIX projectionMatrix)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  MatrixBufferType* dataPtr;
  unsigned int bufferNumber;

  worldMatrix = XMMatrixTranspose(worldMatrix);
  viewMatrix = XMMatrixTranspose(viewMatrix);
  projectionMatrix = XMMatrixTranspose(projectionMatrix);

  result = DeviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
    {
      return false; 
    }

  dataPtr = (MatrixBufferType*)mappedResource.pData;

  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  DeviceContext->Unmap(matrixBuffer, 0);

  bufferNumber = 0;

  DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

  return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* DeviceContext,
			       int indexCount)
{
  DeviceContext->IASetInputLayout(layout);

  DeviceContext->VSSetShader(vertexShader, NULL, 0);
  DeviceContext->PSSetShader(pixelShader, NULL, 0);

  DeviceContext->DrawIndexed(indexCount, 0, 0);
}
