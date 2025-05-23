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
}
