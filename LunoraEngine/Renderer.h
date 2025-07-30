#ifndef RENDERER_H
#define RENDERER_H

#include <d3d11.h>

#include "Application.h"

struct Renderer 
{
  Renderer();
  ~Renderer();

  Initialize();
  Update();
  Shutdown();

  // How to reach Device without global definitions,
  // inheritance,
 
  ID3D11Buffer* CreateVertexBuffer(UINT,
				   bool,
				   bool,
				   D3D11_SUBRESOURCE_DATA);

  ID3D11Buffer* CreateIndexBuffer(UINT,
				  bool,
				  D3D11_SUBRESOURCE_DATA);

  ID3D11Buffer* CreateConstantBuffer(UINT,
				     bool,
				     bool,
				     D3D11_SUBRESOURCE_DATA);


  ID3D11Buffer* CreateStructuredBuffer(UINT,
				       UINT,
				       bool,
				       bool,
				       D3D11_SUBRESOURCE_DATA*);
  
public:
  ID3D11Buffer* m_VertexBuffer;
  ID3D11Buffer* m_IndexBuffer;
  ID3D11Buffer* m_ConstantBuffer; 
};

#endif
