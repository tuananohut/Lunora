#ifndef TERRAINMESHBUILDER_H
#define TERRAINMESHBUILDER_H

#include <vector>
#include <DirectXMath.h>

using namespace DirectX; 

struct TerrainVertex
{
  XMFLOAT3 position;
  XMFLOAT2 uv; 
};

struct TerrainMesh
{
  ID3D11Buffer* vertexBuffer = nullptr;
  ID3D11Buffer* indexBuffer = nullptr;
  UINT vertexCount = 0;
  UINT indexCount = 0;
};

struct TerrainMeshBuilder
{
  static void BuildMesh(const std::vector<float>& heightMap,
			int width,
			int height,
			float spacing,
			std::vector<TerrainVertex>& outVertices,
			std::vector<unsigned int>& outIndices);

  TerrainMesh Build(ID3D11Device* device, const TerrainVertex& data); 
};


#endif
