#include "position.h"

Position::Position()
{
  m_positionX = 0.f;
  m_positionY = 0.f;
  m_positionZ = 0.f;

  m_rotationX = 0.f;
  m_rotationY = 0.f;
  m_rotationZ = 0.f;

  m_frameTime = 0.f;

  m_forwardSpeed = 0.f;
  m_backwardSpeed = 0.f;
  m_upwardSpeed = 0.f;
  m_downwardSpeed = 0.f;
  m_leftTurnSpeed = 0.f; 
  m_rightTurnSpeed = 0.f;
  m_lookUpSpeed = 0.f; 
  m_lookDowndSpeed = 0.f; 
}

Position::Position(const Position& other) {}

Position::~Position() {}

void Position::SetPosition(float x, float y, float z)
{
  m_positionX = x;
  m_positionY = y;
  m_positionZ = z;
}

void Position::SetRotation(float x, float y, float z)
{
  m_rotationX = x;
  m_rotationY = y;
  m_rotationZ = z;
}

void Position::GetPosition(float& x, float& y, float& z)
{
  x = m_positionX;
  y = m_positionY;
  z = m_positionZ;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
  x = m_rotationX;
  y = m_rotationY;
  z = m_rotationZ;
}

void Position::SetFrameTime(float time)
{
  m_frameTime = time; 
}
