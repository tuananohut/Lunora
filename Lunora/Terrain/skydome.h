#ifndef SKYDOME_H
#define SKYDOME_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

/*
// USAGE: &Colors::White
namespace Colors
{
  XMGLOBALCONST XMFLOAT4 White = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Black = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Red = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Green = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Blue = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Yellow = XMFLOAT4(1.f, 1.f, 0.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Cyan = XMFLOAT4(0.f, 1.f, 1.f, 1.f);
  XMGLOBALCONST XMFLOAT4 Magenta = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
}
*/

class SkyDome
{
 private:
  struct ModelType
  {
    float x, y, z;
    float tu, tv;
    float nx, ny, nz; 
  };

  struct VertexType
  {
    XMFLOAT3 position; 
  };

 public:
  SkyDome();
  SkyDome(const SkyDome&);
  ~SkyDome();

  bool Initialize(ID3D11Device*);
  void Shutdown();
  void Render(ID3D11DeviceContext*);

  int GetIndexCount();
  XMFLOAT4 GetApexColor();
  XMFLOAT4 GetCenterColor();

 private:
  bool LoadSkyDomeModel(char*);
  void ReleaseSkyDomeModel();

  bool InitializeBuffers(ID3D11Device*);
  void ReleaseBuffers();
  void RenderBuffers(ID3D11DeviceContext*);

 private:
  ModelType* m_model;
  int m_vertexCount, m_indexCount;
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  XMFLOAT4 m_apexColor, m_centerColor;
  XMFLOAT4 Colors[8]; 
};

#endif
