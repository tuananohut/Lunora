#ifndef SKYDOME_H
#define SKYDOME_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

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
  XMFLOAT4 DayColors[24] =
    {
      XMFLOAT4(0.02f, 0.02f, 0.05f, 1.f),
      XMFLOAT4(0.05f, 0.05f, 0.10f, 1.f),
      XMFLOAT4(0.07f, 0.07f, 0.12f, 1.f),
      XMFLOAT4(0.10f, 0.08f, 0.15f, 1.f),
      XMFLOAT4(0.20f, 0.15f, 0.35f, 1.f),
      XMFLOAT4(0.30f, 0.25f, 0.45f, 1.f),
      XMFLOAT4(0.50f, 0.35f, 0.60f, 1.f),
      XMFLOAT4(0.70f, 0.50f, 0.80f, 1.f),
      XMFLOAT4(0.75f, 0.70f, 0.90f, 1.f),
      XMFLOAT4(0.80f, 0.80f, 1.00f, 1.f),
      XMFLOAT4(0.85f, 0.85f, 1.00f, 1.f),
      XMFLOAT4(0.90f, 0.90f, 1.00f, 1.f),
      XMFLOAT4(0.95f, 0.95f, 1.00f, 1.f),
      XMFLOAT4(0.95f, 0.95f, 1.00f, 1.f),
      XMFLOAT4(0.90f, 0.90f, 1.00f, 1.f),
      XMFLOAT4(0.85f, 0.80f, 0.95f, 1.f),
      XMFLOAT4(0.80f, 0.70f, 0.85f, 1.f),
      XMFLOAT4(0.70f, 0.50f, 0.60f, 1.f),
      XMFLOAT4(0.60f, 0.40f, 0.50f, 1.f),
      XMFLOAT4(0.50f, 0.30f, 0.40f, 1.f),
      XMFLOAT4(0.30f, 0.20f, 0.30f, 1.f),
      XMFLOAT4(0.15f, 0.10f, 0.20f, 1.f),
      XMFLOAT4(0.08f, 0.06f, 0.12f, 1.f),
      XMFLOAT4(0.05f, 0.04f, 0.08f, 1.f)
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
};

#endif
