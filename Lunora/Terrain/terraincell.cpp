#include "terraincell.h"

TerrainCell::TerrainCell()
{
  m_vertexList = nullptr;
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr;
  m_lineVertexBuffer = nullptr;
  m_lineIndexBuffer = nullptr; 
}

TerrainCell::TerrainCell(const TerrainCell &other) {}

TerrainCell::~TerraincCell() {} 

bool TerrainCell::Initialize(ID3D11Device *Device,
			     void *terrainModelPtr,
			     int nodeIndexX,
			     int nodeIndexY,
			     int cellHeight,
			     int cellWidth,
			     int terrainWidth)
{
  ModelType *terrainModel;
  bool result;

  terrainModel = (ModelType*)terrainModelPtr;

  result = InitializeBuffers(Device, nodeIndexX, nodeIndexY, cellHeight, cellWidth, terrainWidth, terrainModel);
  if (!result)
    {
      return false;
    }

  terrainModel = nullptr;

  CalculateCellDimensions();

  result = BuildLineBuffers(Device);
  if (!result)
    {
      return false;
    }

  return true; 
}

void TerrainCell::Shutdown()
{
  ShutdownLineBuffers();

  ShutdownBuffers();
}

void TerrainCell::Render(ID3D11DeviceContext* DeviceContext)
{
  RenderBuffers(DeviceContext); 
}

int TerrainCell::GetVertexCount()
{
  return m_vertexCount; 
}

int TerrainCell::GetIndexCount()
{
  return m_indexCount; 
}

bool TerrainCell::InitializeBuffers(ID3D11Device *device,
				    int nodeIndexX, int nodeIndexY,
				    int cellHeight, int cellWidth,
				    int terrainWidth,
				    ModelType *terrainModel)
{
  VertexType *vertices;
  unsigned long *indices;
  int i, j, modelIndex, index;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  m_vertexCount = (cellHeight - 1) * (cellWidth - 1) * 6;

  m_indexCount = m_vertexCount;

  vertices = new VertexType[m_vertexCount];
  if (!vertices)
    {
      return false; 
    }

  indices = new unsigned long[m_indexCount];
  if (!indices)
    {
      return false; 
    }

  modelIndex = ((nodeIndexX * (cellWidth - 1)) + (nodeIndexY * (cellHeight - 1) * (terrainWidth - 1))) * 6;
  index = 0;

  for(j=0; j<(cellHeight - 1); j++)
    {
      for(i=0; i<((cellWidth - 1) * 6); i++)
	{
	  vertices[index].position = XMFLOAT3(terrainModel[modelIndex].x, terrainModel[modelIndex].y, terrainModel[modelIndex].z);
	  vertices[index].texture = XMFLOAT2(terrainModel[modelIndex].tu, terrainModel[modelIndex].tv);
	  vertices[index].normal = XMFLOAT3(terrainModel[modelIndex].nx, terrainModel[modelIndex].ny, terrainModel[modelIndex].nz);
	  vertices[index].tangent = XMFLOAT3(terrainModel[modelIndex].tx, terrainModel[modelIndex].ty, terrainModel[modelIndex].tz);
	  vertices[index].binormal = XMFLOAT3(terrainModel[modelIndex].bx, terrainModel[modelIndex].by, terrainModel[modelIndex].bz);
	  vertices[index].color = XMFLOAT3(terrainModel[modelIndex].r, terrainModel[modelIndex].g, terrainModel[modelIndex].b);
	  indices[index] = index;
	  modelIndex++;
	  index++;
	}
      modelIndex += (terrainWidth * 6) - (cellWidth * 6);
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
    {
      return false; 
    }

  m_vertexList = new VectorType[m_vertexCount];
  if (!m_vertexList)
    {
      return false; 
    }

  for (i = 0; i < m_vertexCount; i++)
    {
      m_vertexList[i].x = vertices[i].position.x;
      m_vertexList[i].y = vertices[i].position.y;
      m_vertexList[i].z = vertices[i].position.z;
    }

  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr;

  return true; 
}

void TerrainCell::ShutdownBuffers()
{
  if (m_vertexList)
    {
      delete[] m_vertexList;
      m_vertexList = nullptr; 
    }

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
