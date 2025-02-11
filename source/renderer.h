#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <WinUser.h>
#include <wrl/client.h>

#include <windows.h>
#include <d2d1.h>

struct RenderManager2D
{
  ID2D1Factory* Factory;
  ID2D1HwndRenderTarget* RenderTarget;
  ID2D1SolidColorBrush* Brush;
  D2D1_ELLIPSE ellipse;
  HWND Window; 

  void    CalculateLayout();
  HRESULT CreateGraphicsResources();
  void    OnPaint();
  void    Resize();
  void    Shutdown(); 
  void	  GetHwnd(HWND hwnd);
};

#endif
