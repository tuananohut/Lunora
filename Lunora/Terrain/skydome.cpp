#include "skydome.h"

SkyDome::SkyDome()
{
  m_model = nullptr;
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr; 
}

SkyDome::SkyDome(const SkyDome& other) {}

SkyDome::~SkyDome() {}

bool SkyDome::Initialize(ID3D11Device* device)
{
  bool result;

  result = LoadSkyDomeModel("../Lunora/Terrain/skydome.txt");
  if (!result)
    return false;

  result = InitializeBuffers(device);
  if (!result)
    return false;
  
  Colors =
    {
      XMFLOAT4(1.f, 1.f, 1.f, 1.f),
      XMFLOAT4(0.f, 0.f, 0.f, 1.f),  
      XMFLOAT4(1.f, 0.f, 0.f, 1.f),  
      XMFLOAT4(0.f, 1.f, 0.f, 1.f),  
      XMFLOAT4(0.f, 0.f, 1.f, 1.f),   
      XMFLOAT4(1.f, 1.f, 0.f, 1.f), 
      XMFLOAT4(0.f, 1.f, 1.f, 1.f),   
      XMFLOAT4(1.f, 0.f, 1.f, 1.f),
    };
  

  m_apexColor = Colors[3];
  
  m_centerColor = XMFLOAT4(0.2f, 0.5f, 0.8f, 1.f);

  return true;
}

void SkyDome::Shutdown()
{
  ReleaseBuffers();

  ReleaseSkyDomeModel();
}

void SkyDome::Render(ID3D11DeviceContext* deviceContext)
{
  RenderBuffers(deviceContext);
}

int SkyDome::GetIndexCount()
{
  return m_indexCount; 
}

XMFLOAT4 SkyDome::GetApexColor()
{
  return m_apexColor;
}

XMFLOAT4 SkyDome::GetCenterColor()
{
  return m_centerColor;
}

bool SkyDome::LoadSkyDomeModel(char* filename)
{
  ifstream fin;
  char input;
  int i;

  fin.open(filename);

  if (fin.fail())
    {
      return false; 
    }

  fin.get(input);
  while(input != ':')
    {
      fin.get(input);
    }

  fin >> m_vertexCount;

  m_indexCount = m_vertexCount;

  m_model = new ModelType[m_vertexCount];
  if(!m_model)
    {
      return false;
    }

  fin.get(input);
  while(input != ':')
    {
      fin.get(input);
    }
  fin.get(input);
  fin.get(input);

  for (i = 0; i < m_vertexCount; i++)
    {
      fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
      fin >> m_model[i].tu >> m_model[i].tv;
      fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz; 
    }

  fin.close();

  return true; 
}

void SkyDome::ReleaseSkyDomeModel()
{
  if (m_model)
    {
      delete[] m_model;
      m_model = nullptr; 
    }
}

bool SkyDome::InitializeBuffers(ID3D11Device* device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  int i;

  vertices = new VertexType[m_vertexCount];
  if (!vertices)
    return false;

  indices = new unsigned long[m_indexCount];
  if (!indices)
    return false;

  for (i = 0; i < m_vertexCount; i++)
    {
      vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
      indices[i] = i; 
    }

  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
  if (FAILED(result))
    {
      return false; 
    }

  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0; 
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  indexData.pSysMem = indices;
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
  if (FAILED(result))
    return false;

  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr;

  return true; 
}

void SkyDome::ReleaseBuffers()
{
  if (m_indexBuffer)
    {
      m_indexBuffer->Release();
      m_indexBuffer = nullptr;
    }

  if (m_vertexBuffer)
    {
      m_vertexBuffer->Release();
      m_vertexBuffer = nullptr; 
    }
}

void SkyDome::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(VertexType);
  offset = 0;

  deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
