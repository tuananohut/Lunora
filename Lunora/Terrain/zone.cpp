#include "zone.h"

Zone::Zone()
{
  m_UserInterface = nullptr;
  m_Camera = nullptr;
  m_Light = nullptr; 
  m_Position = nullptr;
  m_SkyDome = nullptr; 
  m_Terrain = nullptr; 
}

Zone::Zone(const Zone& other) {}

Zone::~Zone() {}

bool Zone::Initialize(D3D* Direct3D,
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

  m_Light = new Light;
  if (!m_Light)
    {
      return false; 
    }

  m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
  m_Light->SetDirection(-0.5f, -1.f, -0.5f);

  m_Position = new Position; 
  if (!m_Position)
    {
      return false; 
    }

  m_Position->SetPosition(128.f, 10.f, -10.f);
  m_Position->SetRotation(0.f, 0.f, 0.f);

  m_SkyDome = new SkyDome;
  if (!m_SkyDome)
    {
      return false;
    }
  
  result = m_SkyDome->Initialize(Direct3D->GetDevice());
  if (!result)
    {
      MessageBoxA(hwnd, "Could not initialize the sky dome object.", "Error", MB_OK);
      return false; 
    }

  m_apexColor = m_SkyDome->GetApexColor();    
  m_centerColor = m_SkyDome->GetCenterColor();
  
  m_Terrain = new Terrain;
  if (!m_Terrain)
    {
      return false; 
    }

  result = m_Terrain->Initialize(Direct3D->GetDevice(), "../Lunora/Terrain/setup.txt");
  if (!result)
    {
      MessageBoxA(hwnd, "Could not initialize the terrain object.", "Error", MB_OK | MB_ICONERROR);
      return false; 
    }

  m_displayUI = true; 
                      
  m_wireFrame = true;

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

  if (m_SkyDome)
    {
      m_SkyDome->Shutdown();
      delete m_SkyDome;
      m_SkyDome = nullptr; 
    }
  
  if (m_Position)
    {
      delete m_Position;
      m_Position = nullptr; 
    }

  if (m_Light)
    {
      delete m_Light;
      m_Light = nullptr; 
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

inline XMFLOAT4 LerpColor(const XMFLOAT4& a, const XMFLOAT4& b, float t)
{
    return XMFLOAT4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}


bool Zone::Frame(D3D* Direct3D,
		 Input* Input,
		 ShaderManager* ShaderManager,
		 TextureManager* TextureManager,
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
  /*
  static float elapsed = 0.f; 
  static int index = 0;

  elapsed += frameTime;

  if (elapsed > 0.5f)
    {
      elapsed = 0.f;
      index = (index + 1) % 24;
    }

  m_apexColor = m_SkyDome->DayColors[index];
  m_centerColor = m_SkyDome->DayColors[index];
  */

  static float timeOfDay = 6.0f;   // 06:00'dan baþla
  float timeScale = 60.0f;         // 1 saniye = 1 dakika
  timeOfDay += frameTime * (timeScale / 60.0f); // 60 saniye = 1 saat

  if (timeOfDay >= 24.0f)
    timeOfDay -= 24.0f;

  int hour = static_cast<int>(timeOfDay);
  int nextHour = (hour + 1) % 24;
  float t = timeOfDay - static_cast<float>(hour);

  XMFLOAT4 currentColor = LerpColor(m_SkyDome->DayColors[hour], m_SkyDome->DayColors[nextHour], t);

  m_apexColor   = currentColor;
  m_centerColor = currentColor;
    
  result = Render(Direct3D, ShaderManager, TextureManager);
  if (!result)
    {
      return false; 
    }

  return true; 
}

void Zone::HandleMovementInput(Input* Input, float frameTime)
{
  bool keyDown;
  float posX, posY, posZ, rotX, rotY, rotZ;
  
  m_Position->SetFrameTime(frameTime);

  keyDown = Input->IsLeftPressed();
  m_Position->TurnLeft(keyDown);

  keyDown = Input->IsRightPressed();
  m_Position->TurnRight(keyDown);

  keyDown = Input->IsUpPressed();
  m_Position->MoveForward(keyDown);

  keyDown = Input->IsDownPressed();
  m_Position->MoveBackward(keyDown);

  keyDown = Input->IsAPressed();
  m_Position->MoveUpward(keyDown);

  keyDown = Input->IsZPressed();
  m_Position->MoveDownward(keyDown);

  keyDown = Input->IsPgUpPressed();
  m_Position->LookUpward(keyDown);

  keyDown = Input->IsPgDownPressed();
  m_Position->LookDownward(keyDown);

  m_Position->GetPosition(posX, posY, posZ);
  m_Position->GetRotation(rotX, rotY, rotZ);

  m_Camera->SetPosition(posX, posY, posZ);
  m_Camera->SetRotation(rotX, rotY, rotZ);
 
  if (Input->IsF1Toggled())
    {
      m_displayUI = !m_displayUI; 
    }

   if (Input->IsF2Toggled())
    {
      m_wireFrame = !m_wireFrame; 
    }
}

bool Zone::Render(D3D* Direct3D,
		  ShaderManager* ShaderManager,
		  TextureManager* TextureManager)
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
  bool result;
  XMFLOAT3 cameraPosition; 

  m_Camera->Render();

  Direct3D->GetWorldMatrix(worldMatrix);
  m_Camera->GetViewMatrix(viewMatrix);
  Direct3D->GetProjectionMatrix(projectionMatrix);
  m_Camera->GetBaseViewMatrix(baseViewMatrix);
  Direct3D->GetOrthoMatrix(orthoMatrix);

  cameraPosition = m_Camera->GetPosition();

  Direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

  Direct3D->TurnOffCulling();
  Direct3D->TurnZBufferOff();

  worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

  m_SkyDome->Render(Direct3D->GetDeviceContext());
  result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(),
					      m_SkyDome->GetIndexCount(),
					      worldMatrix,
					      viewMatrix, 
					      projectionMatrix,
					      m_apexColor,
					      m_centerColor);
  if(!result)
    {
      return false;
    }

  Direct3D->GetWorldMatrix(worldMatrix);

  Direct3D->TurnZBufferOn();
  Direct3D->TurnOnCulling();
  
  if (m_wireFrame)
    {
      Direct3D->EnableWireframe(); 
    }

  m_Terrain->Render(Direct3D->GetDeviceContext());
  result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(),
					      m_Terrain->GetIndexCount(),
					      worldMatrix,
					      viewMatrix,
					      projectionMatrix,
					      TextureManager->GetTexture(0),
					      TextureManager->GetTexture(1),
					      m_Light->GetDirection(),
					      m_Light->GetDiffuseColor());
  if (!result)
    {
      return false; 
    }

  if (m_wireFrame)
    {
      Direct3D->DisableWireframe(); 
    }

  if (m_displayUI)
    {
      result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
      if (!result)
	{
	  return false; 
	}
    }

  Direct3D->EndScene();

  return true; 
}
