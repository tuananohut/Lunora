#ifndef APPLICATION_H
#define APPLICATION_H

#include "D3d.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.3f;

class Application
{
 public:
  Application();
  Application(const Application&);
  ~Application();

  bool Initialize(int, int, HWND);
  void Shutdown();
  bool Frame();

 private:
  bool Render();

 private:
  D3D *m_Direct3D;  
};

#endif
