#include "Executable.h"
#include "Application.h"

static HostApplication *gHostApplication = NULL; 

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
  gHostApplication = new HostApplication();
  gHostApplication->Go();

  delete (gHostApplication);

  return 0;
}

LRESULT WINAPI EntryMessageHandler(HWND hWnd,
				   UINT Message,
				   WPARAM wParam,
				   LPARAM lParam)
{
  return gHostApplication->MessageHandler(hWnd,
					  Message,
					  wParam,
					  lParam);
}
