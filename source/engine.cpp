#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "StateManager.h"
#include "renderer.h"

const wchar_t CLASS_NAME[] = L"Engine";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    WNDCLASS wc = { };
    RenderManager manager;
    
    StateManager* pState = new (std::nothrow) StateManager;
    if (pState == NULL)
    {
        return 0;
    }

    // Initialize the structure members

    HICON hIcon = (HICON)LoadImage(NULL, L"source/lunora.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE); 
    DWORD wStyles = WS_EX_ACCEPTFILES | WS_EX_TOOLWINDOW | WS_OVERLAPPEDWINDOW;
    WCHAR szExePath[MAX_PATH];
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = hIcon;
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

    HMENU hMenu = NULL; 

    int nDefaultWidth = 640;
    int nDefaultHeight = 480;
    SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
    AdjustWindowRect(&m_rc, WS_OVERLAPPEDWINDOW, (hMenu != NULL) ? true : false);

    hwnd = CreateWindowEx(0,                   
                          CLASS_NAME,          
                          L"Lunora",  
                          wStyles,        
                          x, y, (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
                          NULL,       
                          hMenu,       
                          hInstance,  
                          pState);

    if (hwnd == NULL)
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }


    ShowWindow(hwnd, nCmdShow);

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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    StateManager* pState;
    if (uMsg == WM_CREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pState = reinterpret_cast<StateManager*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
    }
    else
    {
       //  pState = GetAppState(hwnd);
    }

    switch (uMsg)
    {
    case WM_CLOSE: 
    {
        HMENU hMenu;
        hMenu = GetMenu(hwnd);
        if (hMenu != NULL)
        {
            DestroyMenu(hMenu);
        }

        DestroyWindow(hwnd);
        // UnregisterClass(CLASS_NAME, m_hInstance);
        return 0;
    }

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
