#include "zone.h"

Zone::Zone()
{
  m_UserInterface = nullptr;
  m_Camera = nullptr;
  m_Position = nullptr;
  m_Terrain = nullptr; 
}

Zone::Zone(const Zone& other) {}

Zone::~Zone() {}

bool Initialize(D3D* Direct3D,
		HWND hwnd,
		int screenWidth,
		int screenHeight,
		float screenDepth)
{
  bool result;

  m_UserInterface = new UserInterface;
  if (!m_UserInterface)
    {
      return false; 
    }
  
  result = m_UserInterface->Initialize(Direct3D, screenWidth, screenHeight);
  if (!result)
    {
      MessageBoxA(hwnd, "Could not initialize the user interface object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_Camera = new Camera;
  if (!m_Camera)
    {
      return false; 
    }

  m_Camera->SetPosition(0.f, 0.f, -10.f);
  m_Camera->Render();
  m_Camera->RenderBaseViewMatrix();

  m_Position = new Position; 
  if (!m_Position)
    {
      return false; 
    }

  m_Position->SetPosition(128.f, 5.f, -10.f);
  m_Position->SetRotation(0.f, 0.f, 0.f);

  m_Terrain = new Terrain;
  if (!m_Terrain)
    {
      return false; 
    }

  result = m_Terrain->Initialize(Direct3D->GetDevice());
  if (!result)
    {
      MessageBoxA(hwnd, "Could not initialize the terrain object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_displayUI = true;

  return true;
}

void Zone::Shutdown()
{
  if (m_Terrain)
    {
      m_Terrain->Shutdown();
      delete m_Terrain;
      m_Terrain = nullptr;
    }

  if (m_Position)
    {
      m_Position->Shutdown();
      delete m_Position;
      m_Position = nullptr; 
    }

  if (m_Camera)
    {
      delete m_Camera;
      m_Camera = nullptr; 
    }

  if (m_UserInterface)
    {
      m_UserInterface->Shutdown();
      delete m_UserInterface;
      m_UserInterface = nullptr; 
    }
}

bool Zone::Frame(D3D* Direct3D,
		 Input* Input,
		 ShaderManager* ShaderManager,
		 float frameTime,
		 int fps)
{
  bool result;
  float posX, posY, posZ, rotX, rotY, rotZ;

  HandleMovementInput(Input, frameTime);

  m_Position->GetPosition(posX, posY, posZ);
  m_Position->GetRotation(rotX, rotY, rotZ);

  result = m_UserInterface->Frame(Direct3D->GetDeviceContext(),
				  fps,
				  posX, posY, posZ,
				  rotX, rotY, rotZ);
  if (!result)
    {
      return false; 
    }

  result = Render(Direct3D, ShaderManager);
  if (!result)
    {
      return false; 
    }

  return true; 
}
