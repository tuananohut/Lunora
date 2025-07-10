#ifndef ZONE_H
#define ZONE_H

#include "d3d.h"
#include "input.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "timer.h"
#include "userinterface.h"
#include "camera.h"
#include "light.h"
#include "position.h"
#include "terrain.h"

class Zone
{
public:
  Zone();
  Zone(const Zone&);
  ~Zone();

  bool Initialize(D3D*, HWND, int, int, float);
  void Shutdown();
  bool Frame(D3D*, Input*, ShaderManager*, TextureManager*, float, int);  
  
private:
  void HandleMovementInput(Input*, float);
  bool Render(D3D*, ShaderManager*, TextureManager*);

private:
  UserInterface* m_UserInterface;
  Camera* m_Camera;
  Light* m_Light;
  Position* m_Position;
  Terrain* m_Terrain;
  bool m_displayUI, m_wireFrame;
};

#endif
