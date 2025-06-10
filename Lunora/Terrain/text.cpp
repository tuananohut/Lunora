#include "text.h"

Text::Text()
{
  m_vertexBuffer = nullptr;
  m_vertexBuffer2 = nullptr;
  m_indexBuffer = nullptr;
  m_indexBuffer2 = nullptr;
}

Text::Text(const Text& other) {}

Text::~Text() {}

bool Text::Initialize(ID3D11Device* device, 
		      ID3D11DeviceContext* deviceContext, 
		      int screenWidth, 
		      int screenHeight, 
		      int maxLength, 
		      Font* Font, 
		      char* text,
		      int positionX, 
		      int positionY, 
		      float red, 
		      float green, 
		      float blue) 
{
  bool result; 

  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;

  m_maxLength = maxLength;

  m_shadow = shadow; 
  
  result = InitializeBuffers(device, deviceContext, Font, text, positionX, positionY, red, green, blue);
  if (!result)
    {
      return false;
    }

  return true;
}

void Text::Shutdown()
{
  if (m_vertexBuffer)
    {
      m_vertexBuffer->Release();
      m_vertexBuffer = nullptr; 
    }

  if (m_vertexBuffer2)
    {
      m_vertexBuffer2->Release();
      m_vertexBuffer2 = nullptr; 
    }
  
  if (m_indexBuffer)
    {
      m_indexBuffer->Release();
      m_indexBuffer = nullptr; 
    }

  if (m_indexBuffer2)
    {
      m_indexBuffer2->Release();
      m_indexBuffer2 = nullptr; 
    }
}

void Text::Render(ID3D11DeviceContext* deviceContext,
		  ShaderManager* ShaderManager,
		  XMMATRIX worldMatrix,
		  XMMATRIX viewMatrix,
		  XMMATRIX orthoMatrix,
		  ID3D11ShaderResourceView* fontTexture)
{
  RenderSentence(deviceContext, ShaderManager, worldMatrix, viewMatrix, orthoMatrix, fontTexture);
}

bool Text::InitializeSentence(ID3D11Device* device,
			      ID3D11DeviceContext* deviceContext,
			      Font* Font,
			      char* text,
			      int positionX, int positionY,
			      float red, float green, float blue)
{
  VertexType* vertices;
  unsigned long* indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;
  int i;

  m_vertexCount = 6 * m_maxLength;
  m_indexCount = 6 * m_maxLength;
}
