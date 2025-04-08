#include "Camera.h"

Camera::Camera()
{
  m_positionX = 0.f;
  m_positionY = 0.f;
  m_positionZ = 0.f;

  m_rotationX = 0.f;
  m_rotationY = 0.f;
  m_rotationZ = 0.f;  
}

Camera::Camera(const Camera& other) {}

void Camera::SetPosition(float x, float y, float z)
{
  m_positionX = x; 
  m_positionY = y;
  m_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
  m_rotationX = x;
  m_rotationY = y;
  m_rotationZ = z; 
}

XMFLOAT3 Camera::GetPosition()
{
  return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 Camera::GetRotation()
{
  return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void Camera::Render() {}
