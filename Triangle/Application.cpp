#include "Application.h"

Application::Application()
{
  m_Direct3D = nullptr; 
}

Application::Application(const Application& other) {}

Application::~Application() {}

bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
  bool result;

  m_Direct3D = new D3D;

  result = m_Direct3D->Initialize(screenWidth, screenHeight,
				  VSYNC_ENABLED, hwnd,
				  FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
  if (!result)
    {
      MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
      return false; 
    }
  
  return true; 
}

void Application::Shutdown()
{
  if (m_Direct3D)
    {
      m_Direct3D->Shutdown();
      delete m_Direct3D;
      m_Direct3D = nullptr; 
    }
}

bool Application::Frame()
{
  bool result;

  result = Render();
  if (!result)
    {
      return false; 
    }
  
  return true; 
}

bool Application::Render()
{
  m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.f);

  m_Direct3D->EndScene();
  
  return true; 
}

