#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include "d3d.h"
#include "colorshader.h"
#include "textureshader.h"
#include "fontshader.h"

class ShaderManager
{
public:
  ShaderManager();
  ShaderManager(const ShaderManager&);
  ~ShaderManager();

  bool Initialize(ID3D11Device*, HWND);
  void Shutdown();
  
  bool RenderColorShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
  bool RenderTextureShader(ID3D11DeviceContext*, int,
			   XMMATRIX, XMMATRIX, XMMATRIX,
			   ID3D11ShaderResourceView*);
  bool RenderFontShader(ID3D11DeviceContext*, int,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*, XMFLOAT4);
  
private:
  ColorShader* m_ColorShader;
  TextureShader* m_TextureShader;
  FontShader* m_FontShader;
};

#endif
