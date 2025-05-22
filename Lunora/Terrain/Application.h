#ifndef APPLICATION_H
#define APPLICATION_H

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

#include "input.h"
#include "d3d.h"
#include "shadermanager.h"
#include "timer.h"
#include "fps.h"
#include "zone.h"

class Application
{
 public:
  Application();
  Application(const Application&);
  ~Application();

  bool Initialize(HINSTANCE, HWND, int, int);
  void Shutdown();
  bool Frame();

 private:
  Input* m_Input;
  D3D* m_Direct3D;
  ShaderManager* m_ShaderManager;
  Timer* m_Timer;
  Fps* m_Fps;
  Zone* m_Zone; 
};

#endif
