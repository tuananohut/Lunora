#include "terraincell.h"

TerrainCell::TerrainCell()
{
  m_vertexList = nullptr;
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr;
  m_lineVertexBuffer = nullptr;
  m_lineIndexBuffer = nullptr; 
}

TerrainCell::TerrainCell(const TerrainCell& other) {}

TerrainCell::~TerraincCell() {} 
