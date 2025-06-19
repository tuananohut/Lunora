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
    return false;

  result = LoadBitmapHeightMap();
  if (!result)
    return false;

  SetTerrainCoordinates();

  result = BuildTerrainModel();
  if (!result)
    return false;

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
    return false;

  fin.open(filename);
  if (fin.fail())
    return false;

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
    return false;

  error = fopen_s(&filePtr, m_terrainFilename, "rb");
  if (error != 0)
    return false;

  count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
  if (count != 1)
    return false;

  count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
  if (count != 1)
    return false;

  if ((bitmapInfoHeader.biHeight != m_terrainHeight) || (bitmapInfoHeader.biWidth != m_terrainWidth))
    return false;

  imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);

  bitmapImage = new unsigned char[imageSize];
  if (!bitmapImage)
    return false; 

  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

  count = fread(bitmapImage, 1, imageSize, filePtr);
  if (count != imageSize)
    return false;

  error = fclose(filePtr);
  if (error != 0)
    return false;

  k = 0;

  for (j = 0; j < m_terrainHeight < j++)
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
  m_vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 8;
  
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
  
  index = 0;

  for (j = 0; j < (terrainHeight - 1); j++)
    {      for (i = 0; i < (terrainWidth - 1); i++)
	{
	  // Line 1 - Upper Left
	  positionX = (float)i;
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 1 - Upper right
	  positionX = (float)(i + 1);
	  positionZ = (float)(j + 1);
	  
	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 2 - Upper right
	  positionX = (float)(i + 1);
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 2 - Bottom right
	  positionX = (float)(i + 1);
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++; 

	  // Line 3 - Bottom right
	  positionX = (float)(i + 1);
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 3 - Bottom left
	  positionX = (float)i;
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 4 - Bottom left
	  positionX = (float)i;
	  positionZ = (float)j;

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;

	  // Line 4 - Upper left
	  positionX = (float)i;
	  positionZ = (float)(j + 1);

	  vertices[index].position = XMFLOAT3(positionX, 0.f, positionZ);
	  vertices[index].color = color;
	  indices[index] = index;
	  index++;
	}
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

  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); 
}
