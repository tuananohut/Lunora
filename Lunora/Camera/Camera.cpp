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


void Camera::Render()
{
  XMFLOAT3 up, position, lookAt;
  XMVECTOR upVector, positionVector, lookAtVector;
  float yaw, pitch, roll;
  XMMATRIX rotationMatrix;

  up.x = 0.f;
  up.y = 1.f;
  up.z = 0.f; 
  
  upVector = XMLoadFloat3(&up);

  position.x = m_positionX;
  position.y = m_positionY;
  position.z = m_positionZ;

  positionVector = XMLoadFloat3(&position);

  lookAt.x = 0.f;
  lookAt.y = 0.f;
  lookAt.z = 1.f;

  lookAtVector = XMLoadFloat3(&lookAt);

  pitch = m_rotationX * 0.0174532925f;
  yaw = m_rotationY * 0.0174532925f;
  roll = m_rotationZ * 0.0174532925f;
  
  rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

  lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
  upVector = XMVector3TransformCoord(upVector, rotationMatrix);

  lookAtVector = XMVectorAdd(positionVector, lookAtVector);

  m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
  viewMatrix = m_viewMatrix; 
}

void Camera::RenderBaseViewMatrix()
{
  XMFLOAT3 up, position, lookAt;
  XMVECTOR upVector, positionVector, lookAtVector;
  float yaw, pitch, roll;
  XMMATRIX rotationMatrix;

  up.x = 0.f;
  up.y = 1.f;
  up.z = 0.f;

  upVector = XMLoadFloat3(&up);

  position.x = m_positionX;
  position.y = m_positionY;
  position.z = m_positionZ;

  positionVector = XMLoadFloat3(&position);

  lookAt.x = 0.f;
  lookAt.y = 0.f;
  lookAt.z = 1.f;

  lookAtVector = XMLoadFloat3(&lookAt);

  pitch = m_rotationX * 0.0174532925f;
  yaw = m_rotationY * 0.0174532925f;
  roll = m_rotationZ * 0.0174532925f;
	
  rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

  lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
  upVector = XMVector3TransformCoord(upVector, rotationMatrix);

  lookAtVector = XMVectorAdd(positionVector, lookAtVector);

  m_baseViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetBaseViewMatrix(XMMATRIX& viewMatrix)
{
  viewMatrix = m_baseViewMatrix;
}

void Camera::RenderReflection(float height)
{
  XMFLOAT3 up, position, lookAt;
  XMVECTOR upVector, positionVector, lookAtVector;
  float yaw, pitch, roll;
  XMMATRIX rotationMatrix;

  up.x = 0.f;
  up.y = 1.f;
  up.z = 0.f;

  upVector = XMLoadFloat3(&up);

  position.x = m_positionX;
  position.y = -m_positionY + (height * 2.0f);
  position.z = m_positionZ;

  positionVector = XMLoadFloat3(&position);

  lookAt.x = 0.f;
  lookAt.y = 0.f;
  lookAt.z = 1.f;

  lookAtVector = XMLoadFloat3(&lookAt);

  pitch = (-1.f * m_rotationX) * 0.0174532925f;
  yaw = m_rotationY * 0.0174532925f;
  roll = m_rotationZ * 0.0174532925f;

  rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

  lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
  upVector = XMVector3TransformCoord(upVector, rotationMatrix);

  lookAtVector = XMVectorAdd(positionVector, lookAtVector);

  m_reflectionViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetReflectionViewMatrix(XMMATRIX& reflectionViewMatrix)
{
  reflectionViewMatrix = m_reflectionViewMatrix;
}
