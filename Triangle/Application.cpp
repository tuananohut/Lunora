#include "Application.h"

Application::Application()
{
  m_Direct3D = nullptr;
  m_Camera = nullptr;
  m_Model = nullptr;
  m_ColorShader = nullptr; 
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
      MessageBoxA(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
      return false; 
    }

  m_Camera = new Camera;

  m_Camera->SetPosition(0.f, 0.f, -5.f);

  m_Model = new Model;
  result = m_Model->Initialize(m_Direct3D->GetDevice());
  if (!result)
    {
      MessageBoxA(hwnd, "Could not initialize the model object.", "Error", MB_OK);
      return false;
    }

  m_ColorShader = new ColorShader;
  result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
  if (!result)
    {
      MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
      return false;
    }

  return true; 
}

void Application::Shutdown()
{
  if (m_ColorShader)
    {
      m_ColorShader->Shutdown();
      delete m_ColorShader;
      m_ColorShader = nullptr; 
    }

  if (m_Model)
    {
      m_Model->Shutdown();
      delete m_Model;
      m_Model = nullptr; 
    }

  if (m_Camera)
    {
      delete m_Camera;
      m_Camera = nullptr; 
    }
  
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
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
  bool result;
  
  m_Direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

  m_Camera->Render();

  m_Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);;
  m_Direct3D->GetProjectionMatrix(projectionMatrix);

  m_Model->Render(m_Direct3D->GetDeviceContext());
  
  result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
  if (!result)
    {
      return false;
    }
  
  m_Direct3D->EndScene();
  
  return true; 
}

