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

void TerrainCell::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(VertexType);
  offset = 0;

  DeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  DeviceContext->IASetIndexBuffers(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void TerrainCell::CalculateCellDimensions()
{
  int i;
  float width, height, depth;

  m_maxWidth = -1000000.f;
  m_maxHeight = -1000000.f;
  m_maxDepth = -1000000.f;

  m_minWidth = 1000000.f;
  m_minHeight = 1000000.f;
  m_minDepth = 1000000.f;

  for(i = 0; i < m_vertexCount; i++)
    {
      width = m_vertexList[i].x;
      height = m_vertexList[i].y;
      depth = m_vertexList[i].z;

      if(width > m_maxWidth)
	{
	  m_maxWidth = width;
	}
      if(width < m_minWidth)
	{
	  m_minWidth = width;
	}
      
      if(height > m_maxHeight)
	{
	  m_maxHeight = height;
	}
      if(height < m_minHeight)
	{
	  m_minHeight = height;
	}

      if(depth > m_maxDepth)
	{
	  m_maxDepth = depth;
	}
      if(depth < m_minDepth)
	{
	  m_minDepth = depth;
	}
    }
  
  m_positionX = (m_maxWidth - m_minWidth) + m_minWidth;
  m_positionY = (m_maxHeight - m_minHeight) + m_minHeight;
  m_positionZ = (m_maxDepth - m_minDepth) + m_minDepth;
}

bool TerrainCell::BuildLineBuffers(ID3D11Device *Device)
{
  ColorVertexType *vertices;
  unsigned long *indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  XMFLOAT4 lineColor;
  int index, vertexCount, indexCount;

  lineColor = XMFLOAT4(1.f, 0.5f, 0.f, 1.f);

  vertexCount = 24;

  indexCount = vertexCount;

  vertices = new ColorVertexType[vertexCount];
  if (!vertices)
    return false;

  indices = new unsigned long[indexCount];
  if (!indices)
    return false;

  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(ColorVertexType) * vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  indexData.pSysMem = indices;
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  index = 0;
  
  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_maxDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_maxWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_maxHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;
  index++;

  vertices[index].position = XMFLOAT3(m_minWidth, m_minHeight, m_minDepth);
  vertices[index].color = lineColor;
  indices[index] = index;

  result = Device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_lineVertexBuffer);
  if (FAILED(result))
    return false;

  result = Device->CreateBuffer(&indexBufferDesc, &indexData, &m_lineIndexBuffer);
  if (FAILED(result))
    return false;

  m_lineIndexCount = indexCount;

  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr; 
  
  return true;
}

void TerrainCell::ShutdownLineBuffers()
{
  if (m_lineIndexBuffer)
    {
      m_lineIndexBuffer->Release();
      m_lineIndexBuffer = nullptr; 
    }
 
  if (m_lineVertexBuffer)
    {
      m_lineVertexBuffer->Release();
      m_lineVertexBuffer = nullptr; 
    }
}

void TerrainCell::RenderLineBuffers(ID3D11DeviceContext* DeviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(ColorVertexType);
  offset = 0;

  DeviceContext->IASetVertexBuffers(0, 1, &m_lineVertexBuffer, &stride, &offset);

  DeviceContext->IASetIndexBuffer(m_lineIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

  DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); 
}

int TerrainCell::GetLineBuffersIndexCount()
{
  return m_lineIndexCount; 
}

void TerrainCell::GetCellDimensions(float& maxWidth, float& maxHeight, float& maxDepth,
				    float& minWidth, float& minHeight, float& minDepth)
{
  maxWidth = m_maxWidth;
  maxHeight = m_maxHeight;
  maxDepth = m_maxDepth;
  minWidth = m_minWidth;
  minHeight = m_minHeight;
  minDepth = m_minDepth;
}
