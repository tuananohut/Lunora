#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

struct InputHandler
{  
  bool KeyW;
  bool KeyA;
  bool KeyD;
  bool KeyS;

  InputHandler();
  void UpdateInput();
};

#endif
