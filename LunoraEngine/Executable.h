#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <windows.h>

INT WINAPI WinMain(HINSTANCE hInst,
		   HINSTANCE,
		   LPSTR,
		   INT);

LRESULT WINAPI EntryMessageHandler(HWND hWnd,
				   UINT Message,
				   WPARAM wParam,
				   LPARAM lParam);

#endif
