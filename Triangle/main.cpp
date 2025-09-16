#include "system.h"

int WINAPI WinMain(HINSTANCE hInstance,
		   HINSTANCE hPrevInstance,
		   PSTR pScmdline,
		   int iCmdshow)
{
  System *System;
  bool result;

  System = new System;

  result = System->Initialize();
  if (result)
    {
      System->Run();
    }

  System->Shutdown();
  delete System;
  System = nullptr; 
}


