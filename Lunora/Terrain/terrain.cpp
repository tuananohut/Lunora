#include "terrain.h"

Terrain::Terrain()
{
  m_vertexBuffer = nullptr;
  m_indexBuffer = nullptr;
  m_terrainFilename = nullptr;
  m_heightMap = nullptr;
  m_terrainModel = nullptr; 
}

Terrain::Terrain(const Terrain& other) {}

Terrain::~Terrain() {}

bool Terrain::Initialize(ID3D11Device* device, char* setupFilename)
{
  bool result;

  result = LoadSetupFile(setupFilename);
  if (!result)
    {
      return false;
    }
  
  result = LoadBitmapHeightMap();
  if (!result)
    {
      return false;
    }

  SetTerrainCoordinates();

  result = CalculateNormals();
  if (!result)
    {
      return false;
    }

  result = BuildTerrainModel();
  if (!result)
    {
      return false;
    }
  
  ShutdownHeightMap();
  
  result = InitializeBuffers(device);
  if (!result)
    {
      return false; 
    }

  ShutdownTerrainModel();

  return true; 
}

void Terrain::Shutdown()
{
  ShutdownBuffers();

  ShutdownTerrainModel();

  ShutdownHeightMap(); 
}

bool Terrain::Render(ID3D11DeviceContext* deviceContext)
{
  RenderBuffers(deviceContext);

  return true; 
}

int Terrain::GetIndexCount()
{
  return m_indexCount; 
}

bool Terrain::LoadSetupFile(char* filename)
{
  int stringLength;
  ifstream fin;
  char input;

  stringLength = 256;
  m_terrainFilename = new char[stringLength];
  if (!m_terrainFilename)
    {
      return false;
    }
  
  fin.open(filename);
  if (fin.fail())
    {
      return false;
    }
  
  fin.get(input);
  while (input != ':')
    {
      fin.get(input);
    }
  
  fin >> m_terrainFilename;
  
  fin.get(input);
  while(input != ':')
    {
      fin.get(input); 
    }

  fin >> m_terrainHeight;

  fin.get(input);
  while (input != ':')
    {
      fin.get(input);
    }

  fin >> m_terrainWidth;

  fin.get(input);
  while (input != ':')
    {
      fin.get(input); 
    }

  fin >> m_heightScale;

  fin.close();

  return true; 
}

bool Terrain::LoadBitmapHeightMap()
{
  int error, imageSize, i, j, k, index;
  FILE* filePtr;
  unsigned long long count;
  BITMAPFILEHEADER bitmapFileHeader;
  BITMAPINFOHEADER bitmapInfoHeader;
  unsigned char* bitmapImage;
  unsigned char height;

  m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
  if (!m_heightMap)
    {
      return false;
    }
  
  error = fopen_s(&filePtr, m_terrainFilename, "rb");
  if (error != 0)
    {
      return false;
    }
  
  count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
  if (count != 1)
    {
      return false;
    }
  
  count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
  if (count != 1)
    {
      return false;
    }
  
  if ((bitmapInfoHeader.biHeight != m_terrainHeight) || (bitmapInfoHeader.biWidth != m_terrainWidth))
    {
      return false;
    }
  
  imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);

  bitmapImage = new unsigned char[imageSize];
  if (!bitmapImage)
    {
      return false; 
    }
  
  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

  count = fread(bitmapImage, 1, imageSize, filePtr);
  if (count != imageSize)
    {
      return false;
    }
  
  error = fclose(filePtr);
  if (error != 0)
    {
      return false;
    }
  
  k = 0;

  for (j = 0; j < m_terrainHeight; j++)
    {
      for (i = 0; i < m_terrainWidth; i++)
	{
	  index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;

	  height = bitmapImage[k];

	  m_heightMap[index].y = (float)height;

	  k += 3;
	}

      k++;
    }

  delete[]  bitmapImage;
  bitmapImage = nullptr;

  delete[] m_terrainFilename;
  m_terrainFilename = nullptr;

  return true; 
}

void Terrain::ShutdownHeightMap()
{
  if (m_heightMap)
    {
      delete[] m_heightMap;
      m_heightMap = nullptr; 
    }
}

void Terrain::SetTerrainCoordinates()
{
  int i, j, index;

  for (j = 0; j < m_terrainHeight; j++)
    {
      for (i = 0; i < m_terrainWidth; i++)
	{
	  index = (m_terrainWidth * j) + i;

	  m_heightMap[index].x = (float)i;
	  m_heightMap[index].z = -(float)j;

	  m_heightMap[index].z += (float)(m_terrainHeight - 1);

	  m_heightMap[index].y /= m_heightScale; 
	}
    }
}

bool Terrain::CalculateNormals()
{
  int i, j, index1, index2, index3, index;
  float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
  VectorType* normals;

  normals = new VectorType[(m_terrainHeight-1) * (m_terrainWidth-1)];
  if(!normals)
    {
      return false;
    }

  for(j = 0; j < (m_terrainHeight-1); j++)
    {
      for(i = 0; i < (m_terrainWidth-1); i++)
	{
	  index1 = ((j+1) * m_terrainWidth) + i;      
	  index2 = ((j+1) * m_terrainWidth) + (i+1);  
	  index3 = (j * m_terrainWidth) + i;          

	  vertex1[0] = m_heightMap[index1].x;
	  vertex1[1] = m_heightMap[index1].y;
	  vertex1[2] = m_heightMap[index1].z;

	  vertex2[0] = m_heightMap[index2].x;
	  vertex2[1] = m_heightMap[index2].y;
	  vertex2[2] = m_heightMap[index2].z;

	  vertex3[0] = m_heightMap[index3].x;
	  vertex3[1] = m_heightMap[index3].y;
	  vertex3[2] = m_heightMap[index3].z;

	  vector1[0] = vertex1[0] - vertex3[0];
	  vector1[1] = vertex1[1] - vertex3[1];
	  vector1[2] = vertex1[2] - vertex3[2];
	  vector2[0] = vertex3[0] - vertex2[0];
	  vector2[1] = vertex3[1] - vertex2[1];
	  vector2[2] = vertex3[2] - vertex2[2];

	  index = (j * (m_terrainWidth - 1)) + i;

	  normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
	  normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
	  normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);

	  
	  length = (float)sqrt((normals[index].x * normals[index].x) +
			       (normals[index].y * normals[index].y) + 
			       (normals[index].z * normals[index].z));

	  normals[index].x = (normals[index].x / length);
	  normals[index].y = (normals[index].y / length);
	  normals[index].z = (normals[index].z / length);
	}
    }

  for(j = 0; j < m_terrainHeight; j++)
    {
      for(i = 0; i < m_terrainWidth; i++)
	{
	  sum[0] = 0.0f;
	  sum[1] = 0.0f;
	  sum[2] = 0.0f;

	  if(((i-1) >= 0) && ((j-1) >= 0))
	    {
	      index = ((j-1) * (m_terrainWidth-1)) + (i-1);

	      sum[0] += normals[index].x;
	      sum[1] += normals[index].y;
	      sum[2] += normals[index].z;
	    }

	  if((i<(m_terrainWidth-1)) && ((j-1) >= 0))
	    {
	      index = ((j - 1) * (m_terrainWidth - 1)) + i;

	      sum[0] += normals[index].x;
	      sum[1] += normals[index].y;
	      sum[2] += normals[index].z;
	    }

	  if(((i-1) >= 0) && (j<(m_terrainHeight-1)))
	    {
	      index = (j * (m_terrainWidth-1)) + (i-1);

	      sum[0] += normals[index].x;
	      sum[1] += normals[index].y;
	      sum[2] += normals[index].z;
	    }

	  if((i < (m_terrainWidth-1)) && (j < (m_terrainHeight-1)))
	    {
	      index = (j * (m_terrainWidth-1)) + i;

	      sum[0] += normals[index].x;
	      sum[1] += normals[index].y;
	      sum[2] += normals[index].z;
	    }

	  length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

	  index = (j * m_terrainWidth) + i;

	  m_heightMap[index].nx = (sum[0] / length);
	  m_heightMap[index].ny = (sum[1] / length);
	  m_heightMap[index].nz = (sum[2] / length);
	}
    }

  delete[] normals;
  normals = nullptr;

  return true;
}

bool Terrain::BuildTerrainModel()
{
  int i, j, index, index1, index2, index3, index4;
  
  m_vertexCount = (m_terrainHeight - 1) * (m_terrainWidth - 1) * 6;

  m_terrainModel = new ModelType[m_vertexCount];
  if (!m_terrainModel)
    {
      return false; 
    }
  
  index = 0;

  for (j = 0; j < (m_terrainHeight - 1); j++)
    {
      for (i = 0; i < (m_terrainWidth - 1); i++)
	{
	  index1 = (m_terrainWidth * j) + i;
	  index2 = (m_terrainWidth * j) + (i + 1);
	  index3 = (m_terrainWidth * (j + 1)) + i;
	  index4 = (m_terrainWidth * (j + 1)) + (i + 1);

	  m_terrainModel[index].x = m_heightMap[index1].x;
	  m_terrainModel[index].y = m_heightMap[index1].y;
	  m_terrainModel[index].z = m_heightMap[index1].z;
	  m_terrainModel[index].tu = 0.f;
	  m_terrainModel[index].tv = 0.f;
	  m_terrainModel[index].nx = m_heightMap[index1].nx;
	  m_terrainModel[index].ny = m_heightMap[index1].ny;
	  m_terrainModel[index].nz = m_heightMap[index1].nz;
	  index++;
	  
	  m_terrainModel[index].x = m_heightMap[index2].x;
	  m_terrainModel[index].y = m_heightMap[index2].y;
	  m_terrainModel[index].z = m_heightMap[index2].z;
	  m_terrainModel[index].tu = 1.f;
	  m_terrainModel[index].tv = 0.f;
	  m_terrainModel[index].nx = m_heightMap[index2].nx;
	  m_terrainModel[index].ny = m_heightMap[index2].ny;
	  m_terrainModel[index].nz = m_heightMap[index2].nz;
	  index++;

	  m_terrainModel[index].x = m_heightMap[index3].x;
	  m_terrainModel[index].y = m_heightMap[index3].y;
	  m_terrainModel[index].z = m_heightMap[index3].z;
	  m_terrainModel[index].tu = 0.f;
	  m_terrainModel[index].tv = 1.f;
	  m_terrainModel[index].nx = m_heightMap[index3].nx;
	  m_terrainModel[index].ny = m_heightMap[index3].ny;
	  m_terrainModel[index].nz = m_heightMap[index3].nz;
	  index++;

	  m_terrainModel[index].x = m_heightMap[index3].x;
	  m_terrainModel[index].y = m_heightMap[index3].y;
	  m_terrainModel[index].z = m_heightMap[index3].z;
	  m_terrainModel[index].tu = 0.f;
	  m_terrainModel[index].tv = 1.f;
	  m_terrainModel[index].nx = m_heightMap[index3].nx;
	  m_terrainModel[index].ny = m_heightMap[index3].ny;
	  m_terrainModel[index].nz = m_heightMap[index3].nz;
	  index++;
	 
	  m_terrainModel[index].x = m_heightMap[index2].x;
	  m_terrainModel[index].y = m_heightMap[index2].y;
	  m_terrainModel[index].z = m_heightMap[index2].z;
	  m_terrainModel[index].tu = 1.f;
	  m_terrainModel[index].tv = 0.f;
	  m_terrainModel[index].nx = m_heightMap[index2].nx;
	  m_terrainModel[index].ny = m_heightMap[index2].ny;
	  m_terrainModel[index].nz = m_heightMap[index2].nz;
	  index++;
	  
	  m_terrainModel[index].x = m_heightMap[index4].x;
	  m_terrainModel[index].y = m_heightMap[index4].y;
	  m_terrainModel[index].z = m_heightMap[index4].z;
	  m_terrainModel[index].tu = 1.f;
	  m_terrainModel[index].tv = 1.f;
	  m_terrainModel[index].nx = m_heightMap[index4].nx;
	  m_terrainModel[index].ny = m_heightMap[index4].ny;
	  m_terrainModel[index].nz = m_heightMap[index4].nz;
	  index++;
	}
    }

  return true; 
}

void Terrain::ShutdownTerrainModel()
{
  if (m_terrainModel)
    {
      delete[] m_terrainModel;
      m_terrainModel = nullptr;
    }
}

bool Terrain::InitializeBuffers(ID3D11Device* device)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  int i, j, terrainWidth, terrainHeight, index;
  XMFLOAT4 color;
  float positionX, positionZ;

  terrainHeight = 256;
  terrainWidth = 256;

  color = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

  // Drawing a line = 2 points
  // Square has 4 lines
  // Drawing a square 2 points * 4 lines = 8 lines
  m_vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 6;
  
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
  
  for (i = 0; i < m_vertexCount; i++)
    {
      vertices[i].position = XMFLOAT3(m_terrainModel[i].x,
				      m_terrainModel[i].y,
				      m_terrainModel[i].z);
      vertices[i].texture = XMFLOAT2(m_terrainModel[i].tu, m_terrainModel[i].tv);
      vertices[i].normal = XMFLOAT3(m_terrainModel[i].nx,
				    m_terrainModel[i].ny,
				    m_terrainModel[i].nz);
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
    {
      return false; 
    }
  
  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr; 

  return true;
}

void Terrain::ShutdownBuffers()
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

void Terrain::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(VertexType);
  offset = 0;

  deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
  
  deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
}
