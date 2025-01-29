#include <windows.h>
#include "renderer.h"

const wchar_t CLASS_NAME[] = L"Engine";
RenderManager2D manager2D;

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    WNDCLASS wc = { };
    RenderManager manager;

    HICON Icon = (HICON)LoadImage(NULL, L"source/lunora.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE); 
    DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;
    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = Instance;
    wc.hIcon = Icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL; 
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            return HRESULT_FROM_WIN32(dwError);
        }
    }

    RECT m_rc; 

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    HMENU Menu = NULL; 

    int nDefaultWidth = 640;
    int nDefaultHeight = 480;
    SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
    AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, (Menu != NULL) ? true : false);

    Window = CreateWindowEx(0,                   
                            CLASS_NAME,          
                            L"Lunora",  
                            wStyles,        
                            x, y, (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
                            NULL,       
                            Menu,       
                            Instance,  
                            NULL);

    if (Window == NULL)
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    ShowWindow(Window, ShowCode);

    bool bGotMsg; 
    MSG msg = { };
    msg.message = WM_NULL; 
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message)
    {
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            manager.~RenderManager(); 
        }

        else
        {
            // renderer->Update(); 
            // renderer->Render();
            // deviceResources->Present();
        }
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {

    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &manager2D.Factory)))
        {
            return -1;
        }
        return 0;

    case WM_CLOSE: 
    {
        HMENU hMenu;
        hMenu = GetMenu(Window);
        if (hMenu != NULL)
        {
            DestroyMenu(hMenu);
        }

        DestroyWindow(Window);
        // UnregisterClass(CLASS_NAME, m_hInstance);
        return 0;
    }

    case WM_DESTROY:
    {
        manager2D.DiscardGraphicsResources();
        manager2D.SafeRelease(&manager2D.Factory);
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        manager2D.OnPaint();
        return 0;
    }

    case WM_SIZE:
        manager2D.Resize();
        return 0;

    default:
        break;

    }
    
    return DefWindowProc(Window, Message, WParam, LParam);
}
