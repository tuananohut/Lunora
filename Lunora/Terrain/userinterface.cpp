#include "userinterface.h"

UserInterface::UserInterface()
{
  m_Font1 = nullptr;
  m_FpsString = nullptr;
  m_VideoStrings = nullptr;
  m_PositionStrings = nullptr; 
}

UserInterface::UserInterface(const UserInterface& other) {}

UserInterface::~UserInterface() {}

bool UserInterface::Initialize(D3D* Direct3D,
			       int screenWidth,
			       int screenHeight)
{
  bool result;
  char videoCard[128];
  int videoMemory;
  char videoString[144];
  char memoryString[32];
  char tempString[16];
  int i;

  m_Font1 = new Font;
  if (!m_Font)
    {
      return false; 
    }

  result = m_Font1->Initialize(Direct3D->GetDevice(),
			       Direct3D->GetDeviceContext(),
			       "font01.txt", "font01.tga", 32.f, 3);
  if (!result)
    {
      return false; 
    }

  m_FpsString = new Text;
  if (!m_FpsString)
    {
      return false; 
    }

  result = m_FpsString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(),
				   screenWidth, screenHeight, 16, false, m_Font1,
				   "Fps: 0", 10, 50, 0.f, 1.f, 0.f);
  if (!result)
    {
      return false; 
    }

  m_previousFps = -1;

  Direct3D->GetVideoCardInfo(videoCard, videoMemory);
  strcpy_s(videoString, "Video Card: ");
  strcat_s(videoString, videoCard);

  _itoa_s(videoMemory, tempString, 10);

  strcpy_s(memoryString, "Video Memory: ");
  strcat_s(memoryString, tempString);
  strcat_s(memoryString, " MB");

  m_VideoStrings = new Text[2];
  if (!m_VideoStrings)
    {
      return false;
    }

  result = m_VideoStrings[0].Initialize(Direct3D->GetDevice(),
					Direct3D->GetDeviceContext(),
					screenWidth, screenHeight, 256, false, m_Font1,
					videoString, 10, 10, 1.f, 1.f, 1.f);
  if (!result)
    {
      return false; 
    }
  
  result = m_VideoStrings[1].Initialize(Direct3D->GetDevice(),
					Direct3D->GetDeviceContext(),
					screenWidth, screenHeight, 32, false, m_Font1,
					videoString, 10, 30, 1.f, 1.f, 1.f);
  if (!result)
    {
      return false; 
    }
    
  m_PositionStrings = new TextClass[6];
  if(!m_PositionStrings)
    {
      return false;
    }

  result = m_PositionStrings[0].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "X: 0",  10, 310, 1.0f, 1.0f, 1.0f);
  if(!result)
    { 
      return false; 
    }

  result = m_PositionStrings[1].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "Y: 0",  10, 330, 1.0f, 1.0f, 1.0f);
  if(!result)
    { 
      return false; 
    }

  result = m_PositionStrings[2].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "Z: 0",  10, 350, 1.0f, 1.0f, 1.0f);  
  if(!result) 
    { 
      return false; 
    }

  result = m_PositionStrings[3].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "rX: 0", 10, 370, 1.0f, 1.0f, 1.0f);
  if(!result)
    { 
      return false;
    }

  result = m_PositionStrings[4].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "rY: 0", 10, 390, 1.0f, 1.0f, 1.0f);
  if(!result)
    { 
      return false; 
    }

  result = m_PositionStrings[5].Initialize(Direct3D->GetDevice(),
					   Direct3D->GetDeviceContext(),
					   screenWidth, screenHeight, 16, false, m_Font1, 
					   "rZ: 0", 10, 410, 1.0f, 1.0f, 1.0f);  
  if(!result) 
    { 
      return false; 
    }

  for(i = 0; i < 6; i++)
    {
      m_previousPosition[i] = -1;
    }

  return true;
}
