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
