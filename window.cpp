#include "window.h"
extern BOOL running;

GameWindow::GameWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    width = 800, length = 600;
        WNDCLASS wc = { };
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"Sample Window Class";

        RegisterClass(&wc);

        // Create the window.

        this->hwnd = CreateWindowEx(0, wc.lpszClassName, L"SnakeFever", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->length, NULL, NULL, hInstance, NULL);



        ShowWindow(hwnd, nCmdShow);
    

    };
       
    
   HWND GameWindow::getHWND() {
    return(hwnd);
    };






LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
    LRESULT result;
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        running = FALSE;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    }
    return result;
}