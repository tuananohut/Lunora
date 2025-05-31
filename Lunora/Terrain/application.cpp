#include "application.h"

Application::Application()
{
  m_Input = nullptr;
  m_Direct3D = nullptr;
  m_Timer = nullptr;
  m_Fps = nullptr;
  m_ShaderManager = nullptr;
  m_Zone = nullptr; 
}

Application::Application(const Application& other) {}

Application::~Application() {}

bool Application::Initialize(HINSTANCE instance,
			     HWND window,
			     int screenWidth,
			     int screenHeight)
{
  bool result;

  m_Input = new Input;
  if (!m_Input)
    {
      return false;
    }

  result = m_Input->Initialize(instance, window, screenWidth, screenHeight);
  if (!result)
    {
      MessageBoxA(window, "Could not initialize the input object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_Direct3D = new D3D;
  if (!m_Direct3D)
    {
      return false; 
    }

  result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, window, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
  if (!result)
    {
      MessageBoxA(window, "Could not initialize Direct3D.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_ShaderManager = new ShaderManager;
  if (!m_ShaderManager)
    {
      return false; 
    }

  result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), window);
  if (!result)
    {
      MessageBoxA(window, "Could not initialize the shader manager object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_Timer = new Timer;
  if (!m_Timer)
    {
      return false; 
    }

  result = m_Timer->Initialize();
  if (!result)
    {
      MessageBoxA(window, "Could not initialize the timer object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_Fps = new Fps;
  if (!m_Fps)
    {
      return false; 
    }

  m_Fps->Initialize();

  m_Zone = new Zone;
  if (!m_Zone)
    {
      return false; 
    }

  result = m_Zone->Initialize(m_Direct3D, window, screenWidth, screenHeight, SCREEN_DEPTH);
  if (!result)
    {
      MessageBoxA(window, "Could not initialize the zone object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }
  
  return true; 
}

void Application::Shutdown()
{
  if (m_Zone)
    {
      m_Zone->Shutdown();
      delete m_Zone;
      m_Zone = nullptr; 
    }

  if (m_Fps)
    {
      delete m_Fps;
      m_Fps = nullptr; 
    }

  if (m_Timer)
    {
      delete m_Timer;
      m_Timer = nullptr; 
    }

  if (m_ShaderManager)
    {
      m_ShaderManager->Shutdown();
      delete m_ShaderManager;
      m_ShaderManager = nullptr; 
    }

  if (m_Direct3D)
    {
      m_Direct3D->Shutdown();
      delete m_Direct3D;
      m_Direct3D = nullptr; 
    }

  if (m_Input)
    {
      m_Input->Shutdown();
      delete m_Input;
      m_Input = nullptr; 
    }
}

bool Application::Frame()
{
  bool result;

  m_Fps->Frame();
  m_Timer->Frame();
  
  result = m_Input->Frame;
  if (!result)
    {
      return false; 
    }

  if (m_Input->IsEscapePressed() == true)
    {
      return false; 
    }
  
  result = m_Zone->Frame(m_Direct3D, m_Input, m_ShaderManager, m_Timer->GetTime(), m_Fps->GetFps());
  if (!result)
    {
      return false;
    }

  return result;
}
