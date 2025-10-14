#include "FreeCamera.h"

FreeCamera::FreeCamera()
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
  m_lookDownSpeed = 0.f; 
}

FreeCamera::FreeCamera(const FreeCamera& other) {}

FreeCamera::~FreeCamera() {}

void FreeCamera::SetPosition(float x, float y, float z)
{
  m_positionX = x;
  m_positionY = y;
  m_positionZ = z;
}

void FreeCamera::SetRotation(float x, float y, float z)
{
  m_rotationX = x;
  m_rotationY = y;
  m_rotationZ = z;
}

void FreeCamera::GetPosition(float& x, float& y, float& z)
{
  x = m_positionX;
  y = m_positionY;
  z = m_positionZ;
}

void FreeCamera::GetRotation(float& x, float& y, float& z)
{
  x = m_rotationX;
  y = m_rotationY;
  z = m_rotationZ;
}

void FreeCamera::SetFrameTime(float time)
{
  m_frameTime = time; 
}

void FreeCamera::MoveForward(bool keydown)
{
  float radians;
  
  if(keydown)
    {
      m_forwardSpeed += m_frameTime * 1.0f;
      if(m_forwardSpeed > (m_frameTime * 50.0f))
	{
	  m_forwardSpeed = m_frameTime * 50.0f;
	}
    }
  else
    {
      m_forwardSpeed -= m_frameTime * 0.5f;

      if(m_forwardSpeed < 0.0f)
	{
	  m_forwardSpeed = 0.0f;
	}
    }

  radians = m_rotationY * 0.0174532925f;
  
  m_positionX += sinf(radians) * m_forwardSpeed;
  m_positionZ += cosf(radians) * m_forwardSpeed;
}

void FreeCamera::MoveBackward(bool keydown)
{
  float radians;

  if (keydown)
    {
      m_backwardSpeed += m_frameTime * 1.f;

      if (m_backwardSpeed > (m_frameTime * 50.f))
	{
	  m_backwardSpeed = m_frameTime * 50.f;
	}
    }
  else
    {
      m_backwardSpeed -= m_frameTime * 0.5f;

      if (m_backwardSpeed < 0.f)
	{
	  m_backwardSpeed = 0.f; 
	}
    }

  radians = m_rotationY * 0.0174532925f;

  m_positionX -= sinf(radians) * m_backwardSpeed;
  m_positionZ -= cosf(radians) * m_backwardSpeed; 
}

void FreeCamera::MoveUpward(bool keydown)
{
  if(keydown)
    {
      m_upwardSpeed += m_frameTime * 1.5f;

      if(m_upwardSpeed > (m_frameTime * 15.0f))
	{
	  m_upwardSpeed = m_frameTime * 15.0f;
	}
    }
  else
    {
      m_upwardSpeed -= m_frameTime * 0.5f;

      if(m_upwardSpeed < 0.0f)
	{
	  m_upwardSpeed = 0.0f;
	}
    }

  m_positionY += m_upwardSpeed;
}

void FreeCamera::MoveDownward(bool keydown)
{

  if(keydown)
    {
      m_downwardSpeed += m_frameTime * 1.5f;

      if(m_downwardSpeed > (m_frameTime * 15.0f))
	{
	  m_downwardSpeed = m_frameTime * 15.0f;
	}
    }
  else
    {
      m_downwardSpeed -= m_frameTime * 0.5f;

      if(m_downwardSpeed < 0.0f)
	{
	  m_downwardSpeed = 0.0f;
	}
    }

  m_positionY -= m_downwardSpeed;
}

void FreeCamera::TurnLeft(bool keydown)
{
  if(keydown)
    {
      m_leftTurnSpeed += m_frameTime * 5.0f;

      if(m_leftTurnSpeed > (m_frameTime * 150.0f))
	{
	  m_leftTurnSpeed = m_frameTime * 150.0f;
	}
    }
  else
    {
      m_leftTurnSpeed -= m_frameTime* 3.5f;

      if(m_leftTurnSpeed < 0.0f)
	{
	  m_leftTurnSpeed = 0.0f;
	}
    }

  m_rotationY -= m_leftTurnSpeed;

  if(m_rotationY < 0.0f)
    {
      m_rotationY += 360.0f;
    }
}

void FreeCamera::TurnRight(bool keydown)
{
  if(keydown)
    {
      m_rightTurnSpeed += m_frameTime * 5.0f;

      if(m_rightTurnSpeed > (m_frameTime * 150.0f))
	{
	  m_rightTurnSpeed = m_frameTime * 150.0f;
	}
    }
  else
    {
      m_rightTurnSpeed -= m_frameTime* 3.5f;

      if(m_rightTurnSpeed < 0.0f)
	{
	  m_rightTurnSpeed = 0.0f;
	}
    }

  m_rotationY += m_rightTurnSpeed;

  if(m_rotationY > 360.0f)
    {
      m_rotationY -= 360.0f;
    }
}

void FreeCamera::LookUpward(bool keydown)
{
  if(keydown)
    {
      m_lookUpSpeed += m_frameTime * 7.5f;

      if(m_lookUpSpeed > (m_frameTime * 75.0f))
	{
	  m_lookUpSpeed = m_frameTime * 75.0f;
	}
    }
  else
    {
      m_lookUpSpeed -= m_frameTime* 2.0f;

      if(m_lookUpSpeed < 0.0f)
	{
	  m_lookUpSpeed = 0.0f;
	}
    }

  m_rotationX -= m_lookUpSpeed;

	
  if(m_rotationX > 90.0f)
    {
      m_rotationX = 90.0f;
    }
}

void FreeCamera::LookDownward(bool keydown)
{
  if(keydown)
    {
      m_lookDownSpeed += m_frameTime * 7.5f;

      if(m_lookDownSpeed > (m_frameTime * 75.0f))
	{
	  m_lookDownSpeed = m_frameTime * 75.0f;
	}
    }
  else
    {
      m_lookDownSpeed -= m_frameTime* 2.0f;

      if(m_lookDownSpeed < 0.0f)
	{
	  m_lookDownSpeed = 0.0f;
	}
    }

  m_rotationX += m_lookDownSpeed;


  if(m_rotationX < -90.0f)
    {
      m_rotationX = -90.0f;
    }
}
