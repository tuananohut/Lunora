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

