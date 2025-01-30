#include <windows.h>
#include "renderer.h"

RenderManager2D manager2D;
static bool Running; 

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    WNDCLASS wc = {};
    RenderManager manager;

    HICON Icon = (HICON)LoadImage(NULL, L"source/lunora.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE); 
    DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = Instance;
    wc.hIcon = Icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL; 
    wc.lpszClassName = L"Lunora";

    if (!RegisterClass(&wc))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            return HRESULT_FROM_WIN32(dwError);
        }
    }

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    HWND Window = CreateWindowEx(0,                   
                                 wc.lpszClassName,          
                                 L"Lunora",  
                                 wStyles,        
                                 x, y, x, y,
                                 NULL,       
                                 NULL,       
                                 Instance,  
                                 NULL);

    if (Window)
    {
        ShowWindow(Window, ShowCode);
        manager2D.GetHwnd(Window);  

        MSG Message;
        Running = true; 
        while(Running)
        {
            BOOL MessageResult = GetMessageA(&Message, NULL, 0, 0);
            if (MessageResult)
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            }
            else
            {
                // renderer->Update(); 
                // renderer->Render();
                // deviceResources->Present();
                manager.~RenderManager();
                break;
            }
        }
    }
   
    return 0;
}

LRESULT CALLBACK WindowProc(HWND Window, 
                            UINT Message, 
                            WPARAM WParam, 
                            LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
    case WM_CREATE:
    {
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &manager2D.Factory)))
        {
            return -1;
        }
    } break;

    case WM_CLOSE: 
    {
        // TODO: Handle this with a message to the user. 
        Running = false;
    } break;

    case WM_DESTROY:
    {
        Running = false;
        manager2D.Shutdown();
    } break;

    case WM_PAINT:
    {
        manager2D.OnPaint();
    } break;

    case WM_SIZE:
    {
        manager2D.Resize();
    } break;

    default:
    {
        Result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
    }
    
    return Result;
}
