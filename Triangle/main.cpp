#include "system.h"

int WINAPI WinMain(HINSTANCE hInstance,
		   HINSTANCE hPrevInstance,
		   PSTR pScmdline,
		   int iCmdshow)
{
  System *system;
  bool result;
  
  system = new System;

  result = system->Initialize();
  if (result)
    {
      system->Run();
    }

  system->Shutdown();
  delete system;
  system = nullptr; 
}


