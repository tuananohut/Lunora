#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

using namespace DirectX;

// OOP for now

struct Camera
{
  Camera();
  Camera(const Camera&);
  ~Camera();

  void SetPosition(float, float, float);
  void SetRotation(float, float, float);
  
  XMFLOAT3 GetPosition();
  XMFLOAT3 GetRotation();

  void Render();
  void GetViewMatrix(XMMATRIX&);

  float m_positionX, m_positionY, m_positionZ;
  float m_rotationX, m_rotationY, m_rotationZ;
  float focal_length; 
  XMMATRIX m_viewMatrix;
}; 

#endif
