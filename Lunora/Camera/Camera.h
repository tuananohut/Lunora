#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

using namespace DirectX;

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

  void RenderBaseViewMatrix();
  void GetBaseViewMatrix(XMMATRIX&);

  void RenderReflection(float);
  void GetReflectionViewMatrix(XMMATRIX&);
  
  float m_positionX, m_positionY, m_positionZ;
  float m_rotationX, m_rotationY, m_rotationZ;
  XMMATRIX m_viewMatrix;
  XMMATRIX m_baseViewMatrix;
  XMMATRIX m_reflectionMatrix; 
}; 

#endif
