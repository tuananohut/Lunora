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
      MessageBox(window, L"Could not initialize the input object.", L"Error", MB_OK | MB_ICONERROR);
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
      MessageBox(window, L"Could not initialize Direct3D.", L"Error", MB_OK | MB_ICONERROR);
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
      MessageBox(window, L"Could not initialize the shader manager object.", L"Error", MB_OK | MB_ICONERROR);
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
      MessageBox(window, L"Could not initialize the timer object.", L"Error", MB_OK | MB_ICONERROR);
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
      MessageBox(window, L"Could not initialize the zone object.", L"Error", MB_OK | MB_ICONERROR);
      return false; 
    }
  
  return true; 
}

