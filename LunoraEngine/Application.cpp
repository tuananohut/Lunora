#include "Executable.h"
#include "Application.h"

HostApplication::HostApplication()
{
  m_WindowWidth = 640;
  m_WindowHeight = 480;
}

void HostApplication::Go()
{
  WNDCLASSEX WindowClass =
    {
      sizeof(WNDCLASSEX),
      CS_CLASSDC,
      EntryMessageHandler,
      0,
      0,
      GetModuleHandle(NULL),
      NULL,
      NULL,
      NULL,
      NULL,
      "Host Application",
      NULL
    };

  RegisterClassEx(&WindowClass);

  m_hWnd = CreateWindow("Host Application",
			"Host Application",
			WS_OVERLAPPEDWINDOW,
			0, 0,
			m_WindowWidth, m_WindowHeight,
			GetDesktopWindow(),
			NULL,
			WindowClass.hInstance,
			NULL);

  if (m_hWnd)
    {
      ShowWindow(m_hWnd, SW_SHOW);
    }
  else
    {
      return; 
    }

  MSG Message;
  PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
  while(Message.message != WM_QUIT)
    {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
      PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
    }
}

HostApplication::~HostApplication()
{
  if (m_hWnd)
    {
      DestroyWindow(m_hWnd);
    }
}

LRESULT WINAPI HostApplication::MessageHandler(HWND hWnd,
					       UINT Message,
					       WPARAM wParam,
					       LPARAM lParam)
{
  switch(Message)
    {
    case WM_DESTROY:
      {
	PostQuitMessage(0);
      } break; 
    }

  return DefWindowProc(hWnd, Message, wParam, lParam);
}


