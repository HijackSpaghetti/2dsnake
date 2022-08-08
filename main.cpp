#include <windows.h>

#include "renderer.cpp"
//#include "imageProc.cpp"



BOOL running;
int linput;
Renderer rend;
int x, y;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    RECT rect;//to delete

    int WindowWidth = 800, WindowHeight = 800;

    WNDCLASS WindowClass = { };
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = L"SnakeFeverWindow";
   // WindowClass.hIcon=
    //WindowClass.hCursor=
    WindowClass.hbrBackground = 0;
    RegisterClass(&WindowClass);

    // Create the window.
    HWND hwnd = CreateWindowEx(0, L"SnakeFeverWindow", L"SnakeFever", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    if (hwnd == NULL)
        return 0;

    rend.init(hInstance, hwnd, GetWindowDC(hwnd));

    running = TRUE;
    linput = 0; 
    x = 0, y = 0;
    image dot, owl,testpng;
    MSG msg = { };
  //  dot.loadBMP("pictures/dot.bmp");
    owl.loadBMP("pictures/owl.bmp");
    testpng.loadPNG("pictures/Untitled2.png");

    while (running) {

        //input    
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
         //simulate



         //render
        rend.fillAll(0xff000000); //fill opaque black 
        
        //rend.drawPoint(, 0xff0000);
        //rend.drawLine(0, 0,x,y,0xff0000);
        //rend.drawRect(0, 0, x, y, 0xff0000);
        rend.drawImage(testpng, x, y, 0, 0);
        rend.drawRect(0, 0, rend.getWidth() - 1, rend.getHeight() - 1, 0xff0000);

        
      //  rend.drawScreen();


        //UpdateWindow(hwnd);

    }



    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result=0;
    switch (uMsg)
    {
    case WM_KEYDOWN: {
        switch (wParam) {
            case VK_LEFT:
                x--;
                break;
            case VK_RIGHT:
                x++;
                break;
            case VK_UP:
                y--;
                break;
            case VK_DOWN:
                y++;
                break;
            default:
                break;
        }
    
    }
    break;
    case WM_LBUTTONDOWN: {
      //  x = LOWORD(lParam);
       // y = HIWORD(lParam);
    }
    break;
    case WM_MOUSEMOVE: {
       // int mx= LOWORD(lParam),my= HIWORD(lParam);
        //rend.drawRect( mx- 5,my-5, mx + 5, my +5, 0xff0000);
    }
                     break;
    case WM_DESTROY:
    {
       running = FALSE;
    }
    break;
    case WM_PAINT:
    {
       rend.drawScreen();
    }
    break;
    case WM_SIZE:
    {

  
       rend.resize();
       
    }
    break;
    
    default: {
       result= DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}

