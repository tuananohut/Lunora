#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"

namespace Lunora {
  
  struct LUNORA_API Application
    {
      Application();
      virtual ~Application();
      
      void Run();
    };
  
  Application* CreateApplication();
  
}

#endif
