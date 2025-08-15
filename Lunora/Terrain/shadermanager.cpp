#include "shadermanager.h"

ShaderManager::ShaderManager()
{
  m_ColorShader = nullptr;
  m_TextureShader = nullptr;
  m_LightShader = nullptr; 
  m_FontShader = nullptr;
  m_SkyDomeShader = nullptr; 
  m_TerrainShader = nullptr; 
}

ShaderManager::ShaderManager(const ShaderManager& other) {}

ShaderManager::~ShaderManager() {}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
  bool result;
  
  m_ColorShader = new ColorShader;
  if (!m_ColorShader)
    return false;
  result = m_ColorShader->Initialize(device, hwnd);
  if (!result)
    return false;

  m_TextureShader = new TextureShader;
  if (!m_TextureShader)
    return false;
  result = m_TextureShader->Initialize(device, hwnd);
  if (!result)
    return false;

  m_LightShader = new LightShader;
  if (!m_LightShader)
    return false;
  result = m_LightShader->Initialize(device, hwnd);
  if (!result)
    return false; 
  
  m_FontShader = new FontShader;
  if (!m_FontShader)
    return false; 
  result = m_FontShader->Initialize(device, hwnd);
  if (!result)
    return false;

  m_SkyDomeShader = new SkyDomeShader;
  if(!m_SkyDomeShader)
    return false;
  result = m_SkyDomeShader->Initialize(device, hwnd);
  if(!result)
    return false;

  m_TerrainShader = new TerrainShader;
  if (!m_TerrainShader)
    return false;
  result = m_TerrainShader->Initialize(device, hwnd);
  if (!result)
    return false;
  
  return true;
}

void ShaderManager::Shutdown()
{
  if (m_TerrainShader)
    {
      m_TerrainShader->Shutdown();
      delete m_TerrainShader;
      m_TerrainShader = nullptr;
    }

  if (m_SkyDomeShader)
    {
      m_SkyDomeShader->Shutdown();
      delete m_SkyDomeShader;
      m_SkyDomeShader = nullptr;
    }
  
  if (m_FontShader)
    {
      m_FontShader->Shutdown();
      delete m_FontShader;
      m_FontShader = nullptr; 
    }

  if (m_LightShader)
    {
      m_LightShader->Shutdown();
      delete m_LightShader;
      m_LightShader = nullptr; 
    }

  if (m_TextureShader)
    {
      m_TextureShader->Shutdown();
      delete m_TextureShader;
      m_TextureShader = nullptr; 
    }

  if (m_ColorShader)
    {
      m_ColorShader->Shutdown();
      delete m_ColorShader;
      m_ColorShader = nullptr; 
    }
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext* deviceContext,
				      int indexCount,
				      XMMATRIX worldMatrix,
				      XMMATRIX viewMatrix,
				      XMMATRIX projectionMatrix)
{
  return m_ColorShader->Render(deviceContext, indexCount,
			       worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext,
					int indexCount,
					XMMATRIX worldMatrix,
					XMMATRIX viewMatrix,
					XMMATRIX projectionMatrix,
					ID3D11ShaderResourceView* texture)
{
  return m_TextureShader->Render(deviceContext, indexCount,
				 worldMatrix,
				 viewMatrix,
				 projectionMatrix,
				 texture); 
}

bool ShaderManager::RenderLightShader(ID3D11DeviceContext* deviceContext,
				      int indexCount,
				      XMMATRIX worldMatrix,
				      XMMATRIX viewMatrix,
				      XMMATRIX projectionMatrix,
				      ID3D11ShaderResourceView* texture,
				      XMFLOAT3 lightDirection,
				      XMFLOAT4 diffuseColor)
{
  return m_LightShader->Render(deviceContext, indexCount,
			       worldMatrix,
			       viewMatrix,
			       projectionMatrix,
			       texture,
			       lightDirection,
			       diffuseColor);
}

bool ShaderManager::RenderFontShader(ID3D11DeviceContext* deviceContext,
				     int indexCount,
				     XMMATRIX worldMatrix,
				     XMMATRIX viewMatrix,
				     XMMATRIX projectionMatrix,
				     ID3D11ShaderResourceView* texture,
				     XMFLOAT4 color)
{
  return m_FontShader->Render(deviceContext, indexCount,
			      worldMatrix, viewMatrix, projectionMatrix,
			      texture, color);
}

bool ShaderManager::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext,
					int indexCount,
					XMMATRIX worldMatrix,
					XMMATRIX viewMatrix, 
					XMMATRIX projectionMatrix,
					XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	return m_SkyDomeShader->Render(deviceContext, indexCount,
				       worldMatrix, viewMatrix, projectionMatrix,
				       apexColor, centerColor);
}

bool ShaderManager::RenderTerrainShader(ID3D11DeviceContext* deviceContext,
					int indexCount,
					XMMATRIX worldMatrix,
					XMMATRIX viewMatrix,
					XMMATRIX projectionMatrix,
					ID3D11ShaderResourceView* texture,
					ID3D11ShaderResourceView* normalMap,
					XMFLOAT3 lightDirection,
					XMFLOAT4 diffuseColor)
{
  return m_TerrainShader->Render(deviceContext, indexCount,
				 worldMatrix, viewMatrix, projectionMatrix,
				 texture, normalMap,
				 lightDirection, diffuseColor);
}
