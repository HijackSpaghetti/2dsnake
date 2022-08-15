#include <windows.h>

#include "renderer.cpp"
//#include "imageProc.cpp"



BOOL running;
int linput;
Renderer rend;
int x, y;
byte alpha;
double size;
double theta;
int f;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{


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
    x = 60, y = 80;
    alpha = 0;
    size = 1;
    image dot, owl,testpng,frame,colors;
    MSG msg = { };
  //  dot.loadBMP("pictures/dot.bmp");
    //owl.loadBMP("pictures/owl.bmp");
    //testpng.loadPNG("pictures/greentransparent.png");
    //frame.loadPNG("pictures/frame.png");
    colors.loadPNG("pictures/font2.png");
    //bigcolors.imgResize(x, y);
    sprite bigcolors(colors,90,100,20,20,20,20);
    theta= 0;
   // bigcolors.imgRotate(0);
    f = 0;
    while (running) {
        
        //input    
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
         //simulate
        theta -= 0.001;


         //render
       rend.fillAll(0x000000ff); //fill opaque black 
        
        //rend.drawPoint(, 0xff0000);
        //rend.drawLine(0, 0,x,y,0xff0000);
        //rend.drawRect(0, 0, x, y, 0xff0000);
       // rend.drawImage(owl, 0, 0, 0, 0);
      //  bigcolors.imgResize(size);
      //bigcolors.imgRotate(-theta);//20*3.14/180
    //bigcolors.imgHue(0x22ffffff);
       bigcolors.set_frame_number(f);
       bigcolors.resizeNN(size);
       bigcolors.rotate(theta);
       bigcolors.addHue(0x80808080);
        rend.drawSprite(bigcolors, x, y, 0, 0, 0, 0, alpha);


        
        //rend.drawScreen();




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
                //x -= 25;
                f--;
                break;
            case VK_RIGHT:
                //x+=25;
                f++;

                break;
            case VK_UP:
                y-=25;
                break;
            case VK_DOWN:
                y+=25;
                break;
            case 'X':
                if (alpha<0xff)
                theta += 0.003;
                break;
            case 'Z':
                theta -= 0.003;
                break;
            case 'A':
                if (size > 0.01)
                    size-=0.1;
                break;
            case 'S':
                if (size < 2.01)
                    size+=0.1;
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

