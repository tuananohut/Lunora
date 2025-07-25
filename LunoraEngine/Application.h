#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>

class HostApplication
{
 public:
  HostApplication();
  ~HostApplication();

  LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  void Go(); 

 protected:
  HWND m_hWnd;
  long m_WindowWidth;
  long m_WindowHeight; 
};

#endif
