#ifndef UNICODE
#define UNICODE
#endif 

// https://www.flaticon.com/free-icon/cube_9853406?term=shape&page=1&position=93&origin=search&related_id=9853406

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Engine";
    HICON hIcon = (HICON)LoadImage(NULL, L"source/lunora.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE); 
    DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;

    WNDCLASS wc = { };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = hIcon;
    wc.hCursor = 0;
    wc.hbrBackground = HBRUSH(COLOR_WINDOW);
    wc.lpszMenuName = NULL; 
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0,                   
                               CLASS_NAME,          
                               L"Physics Engine",  
                               wStyles,        
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               NULL,       
                               NULL,       
                               hInstance,  
                               NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
        
    }

    default:
        break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    
}