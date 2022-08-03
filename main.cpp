#include <windows.h>

#include "renderer.cpp"



BOOL running;
int linput;
Renderer rend;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{   

    int WindowWidth = 800, WindowHeight = 600;

    WNDCLASS WindowClass = { };
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = L"SnakeFeverWindow";
    WindowClass.hbrBackground = 0;
    RegisterClass(&WindowClass);

    // Create the window.
    HWND hwnd = CreateWindowEx(0, L"SnakeFeverWindow", L"SnakeFever", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    if (hwnd == NULL)
        return 0;

    rend.init(hInstance, hwnd, GetWindowDC(hwnd), WindowWidth,WindowHeight);

    running = TRUE;
    linput = 0; 
    MSG msg = { };
    while (running) {

        //input    
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
         //simulate



         //render
        rend.drawPoint(22, 22, 22);
        rend.drawScreen();
        //UpdateWindow(hwnd);

    }



    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
    case WM_KEYDOWN: {
        switch (wParam) {
            case VK_LEFT:
            linput++;
                break;
            case VK_RIGHT:
                running = FALSE;
                break;
            case VK_UP:

                break;
            case VK_DOWN:

                break;
            default:
                break;
        }
    
    }
    break;
    case WM_DESTROY:
    {
       running = FALSE;
    }
    break;
    case WM_PAINT:
    {
       //rend.drawScreen();
    }
    break;
    case WM_SIZE:
    {
       rend.resize(LOWORD(lParam), HIWORD(lParam));
       return 0;
    }
    break;
    
    default: {
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

