#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include "d3d.h"
#include "colorshader.h"
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
  bool RenderFontShader(ID3D11DeviceContext*, int,
			XMMATRIX, XMMATRIX, XMMATRIX,
			ID3D11ShaderResourceView*, XMFLOAT4);
  
private:
  ColorShader *m_ColorShader;
  FontShader *m_FontShader;
};

#endif
