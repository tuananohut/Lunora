#include "TerrainMeshBuilder.h"

void TerrainMeshBuilder::BuildMesh(const std::vector<float>& heightMap,
				   int width,
				   int height,
				   float spacing,
				   std::vector<TerrainVertex>& outVertices,
				   std::vector<unsigned int>& outIndices)
{
  outVertices.clear();
  outIndices.clear();

  for (int z = 0; z < height; ++z)
    {
      for (int x = 0; x < width; ++x)
	{
	  float y = heightMap[z * width + x];
	  TerrainVertex vertex;
	  vertex.position = XMFLOAT3(x * spacing, y, z * spacing);
	  vertex.uv = XMFLOAT2((float)x / (width - 1), (float)z / (height - 1));
	  outVertices.push_back(vertex);
	}
    }

  for (int z = 0; z < height - 1; ++z)
    {
      for (int x = 0; x < width - 1; ++x)
	{
	  int topLeft = z * width + x;
	  int topRight = topLeft + 1;
	  int bottomLeft = (z + 1) * width + x;
	  int bottomRight = bottomLeft + 1;

	  outIndices.push_back(topLeft);
	  outIndices.push_back(bottomLeft);
	  outIndices.push_back(topRight);

	  outIndices.push_back(topRight);
	  outIndices.push_back(bottomLeft);
	  outIndices.push_back(bottomRight);
	}
    }
}

TerrainMesh TerrainMeshBuilder::Build(ID3D11Device* device, const TerrainVertex& data)
{
  std::vector<TerrainVertex> vertices;
  std::vector<unsigned int> indices;
  
  BuildMesh(data.heightMap, data.width, data.height, data.spacing, vertices, indices);
  
  TerrainMesh mesh;
  mesh.vertexCount = static_cast<UINT>(vertices.size());
  mesh.indexCount  = static_cast<UINT>(indices.size());

  D3D11_BUFFER_DESC vbd = {};
  vbd.Usage = D3D11_USAGE_DEFAULT;
  vbd.ByteWidth = static_cast<UINT>(sizeof(TerrainVertex) * vertices.size());
  vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA vinitData = {};
  vinitData.pSysMem = vertices.data();

  device->CreateBuffer(&vbd, &vinitData, &mesh.vertexBuffer);
   
  D3D11_BUFFER_DESC ibd = {};
  ibd.Usage = D3D11_USAGE_DEFAULT;
  ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * indices.size());
  ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA iinitData = {};
  iinitData.pSysMem = indices.data();

  device->CreateBuffer(&ibd, &iinitData, &mesh.indexBuffer);

  return mesh;
}

