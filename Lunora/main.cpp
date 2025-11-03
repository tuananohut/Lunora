#include <windows.h>

#include "resource.h"
#include "../Lunora/Engine/Renderer.h"
#include "../Lunora/Game/Camera/Camera.h"
#include "../Lunora/Rendering/Mesh.h"
#include "../Lunora/Rendering/Shader/Shader.h"
#include "../Lunora/Rendering/Texture.h"

const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

static bool Running; 

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
  LRESULT Result = 0;
 
  switch (Message)
    {  
    case WM_CLOSE: 
      {
        Running = false;
      } break;

    case WM_DESTROY:
      {			
	Running = false;		
      } break;

    default:
      {
	Result = DefWindowProc(Window, Message, WParam, LParam);
      } break;
    }
    
  return Result;
}

int WINAPI WinMain(HINSTANCE Instance,
		   HINSTANCE PrevInstance,
		   LPSTR CommandLine,
		   int ShowCode)
{
  Win32WindowProperties* Window = new Win32WindowProperties;
  Window->Width = 800;
  Window->Height = 600;
	  
  
  WNDCLASSEXA wc = {};
  
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = Instance;
  wc.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_ICON1));;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "Lunora";

  HRESULT result; 
  
  if (RegisterClassExA(&wc))
    {
      int x = Window->Width;
      int y = Window->Height;
 
      Window->hwnd = CreateWindowExA(0,                   
				     wc.lpszClassName,          
				     "Lunora",  
				     WS_OVERLAPPEDWINDOW | WS_VISIBLE,        
				     CW_USEDEFAULT, CW_USEDEFAULT, x, y, 
				     NULL,       
				     NULL,       
				     Instance,  
				     NULL);
      
      
  
      if (Window->hwnd)
	{	  
	  Running = true;
	  
	  RendererContext *Renderer = new RendererContext;
	  Running = InitializeRenderer(*Renderer, Window->hwnd, Window->Width, Window->Height);
	  if (!Running)
	    {
	      MessageBoxA(Window->hwnd, "Worked!", "Good", MB_OK);
	      Running = false;
	      return 0; 
	    }
	  
	  Camera *mCamera = new Camera;
	  mCamera->SetPosition(0.0f, 0.0f, -15.0f);
	  
	  Mesh *triangle = new Mesh;
	  char filename[] = "../Assets/Models/triangle.txt";	  
	  Running = InitializeModel(Renderer->Device, triangle, filename);
	  if (!Running)
	    {
	      MessageBoxA(Window->hwnd, "Worked!", "Good", MB_OK);
	      Running = false;
	      return 0; 
	    }

	  Mesh *cube = new Mesh;
	  char filename1[] = "../Assets/Models/cube.txt";
	  Running = InitializeModel(Renderer->Device, cube, filename1);
	  if (!Running)
	    {
	      MessageBoxA(Window->hwnd, "Worked!", "Good", MB_OK);
	      Running = false;
	      return 0; 
	    }

	  ColorShader shader; 
	  if (FAILED(InitializeShaderResources(*Renderer, shader)))
	    {
	      MessageBoxA(Window->hwnd, "Something is wrong!", "Bad", MB_OK | MB_ICONERROR);
	      Running = false;
	      return 0; 
	    }

	  TextureShader texShader;
	  if (!InitializeShaderResources(*Renderer, texShader))
	    {
	      MessageBoxA(Window->hwnd, "Something is wrong!", "Bad", MB_OK | MB_ICONERROR);
	      Running = false;
	      return 0; 
	    }

	  
	  Texture* texture = new Texture;
	  const char* texture_file = "../Assets/Textures/palestine.tga";  
	  result = InitializeTexture(Renderer->Device,
				     Renderer->DeviceContext,
				     texture, texture_file);
	  if (FAILED(result))
	    {
	      MessageBoxA(Window->hwnd, "Something is wrong about textures!", "Bad", MB_OK | MB_ICONERROR);
	      Running = false;
	      return 0; 
	    }
	  
	  LARGE_INTEGER frequency;
	  LARGE_INTEGER startTime;
	  QueryPerformanceFrequency(&frequency); 
	  QueryPerformanceCounter(&startTime);

	  float rotationSpeed = 2.0f;
	  
	  while(Running)
	    {
	      MSG Message;
	      while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
		{		  
		  if (Message.message == WM_QUIT)
		    {
		      delete Window; 
		      if (mCamera)
			{
			  delete mCamera;
			  mCamera = nullptr; 
			}

		      if (texture)
			{
			  ReleaseTexture(texture);
			  delete texture;
			  texture = nullptr; 
			}
		      
		      if (triangle)
			{
			  ReleaseModel(triangle); 
			  delete triangle;
			  triangle = nullptr; 
			} 

		      if (cube)
			{
			  ReleaseModel(cube); 
			  delete cube;
			  cube = nullptr; 
			} 
		      
		      if (Renderer)
			{
			  ShutdownRenderer(*Renderer);
			  delete Renderer;
			  Renderer = nullptr; 
			}
		      
		      ReleaseShaderResources(shader);
		      ReleaseShaderResources(texShader);
		      
		      Running = false;
		    }
		  
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	           
	      RendererBeginScene(*Renderer, 0.f, 0.f, 0.f, 1.f);

	      mCamera->Render();

	      LARGE_INTEGER currentTime;
	      QueryPerformanceCounter(&currentTime);

	      float elapsedTime = (float)(currentTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;
	      
	      XMMATRIX world = XMMatrixRotationX(elapsedTime * 0.7f) * XMMatrixRotationY(elapsedTime);

	      // XMMATRIX world = XMMatrixIdentity();
	      XMMATRIX view;	      
	      mCamera->GetViewMatrix(view);
  
	      float fieldOfView = 3.141592654f / 4.0f;
	      float screenAspect = 1.f; 
	      if (windowHeight > 0)
		{
		  screenAspect = (float)windowWidth / (float)windowHeight;
		}
	      else
		{
		  screenAspect = 1.0f; 
		}
	      
	      XMMATRIX proj  = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	      
	      RenderModel(*Renderer, cube);
	      result = Render(*Renderer, texShader, cube->indexCount, world, view, proj, texture->m_textureView);
	      if (FAILED(result))
		{
	       Running = false; 
		}

	      world = XMMatrixIdentity();
	      world *= XMMatrixTranslation(-8.f, 0.f, 0.f);
	      world *= XMMatrixRotationY(elapsedTime * 0.7f) * XMMatrixRotationX(elapsedTime);
	      
	      RenderModel(*Renderer, triangle);	      
	      result = Render(*Renderer, shader, triangle->indexCount, world, view, proj);
	      if (FAILED(result))
		{
	       Running = false; 
		}
	      
	      RendererEndScene(*Renderer);
	    }  
	} 
    }
  
  return 0;
}
