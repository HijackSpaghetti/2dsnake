#include <windows.h>
#include "renderer.cpp"
#include "input.cpp"




static BOOL running;
static Renderer rend;
static input in;

class engine {
private:
    HWND hwnd;
    int createWindow() {
        int WindowWidth = 800, WindowHeight = 800;

        WNDCLASS WindowClass;
        WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        WindowClass.lpfnWndProc = WindowProc;
        WindowClass.hInstance = GetModuleHandle(nullptr);
        WindowClass.lpszClassName = L"SnakeFeverWindow";
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        WindowClass.lpszMenuName = nullptr;
        WindowClass.hbrBackground = nullptr;
        RegisterClass(&WindowClass);

        // Create the window.
        hwnd = CreateWindowEx(0, L"SnakeFeverWindow", L"SnakeFever", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, NULL, NULL, WindowClass.hInstance, this);
        ShowWindow(hwnd, 5);
        if (hwnd == NULL)
            return 0;

        rend.init(WindowClass.hInstance, hwnd, GetWindowDC(hwnd));
        return 0;
    };
public:


    
    int start() {
        createWindow();
        image dot, owl, testpng, frame, colors;
        MSG msg = { };
        running = true;
        dot.loadBMP("pictures/dot.bmp");
        //owl.loadBMP("pictures/owl.bmp");
        //testpng.loadPNG("pictures/greentransparent.png");
        //frame.loadPNG("pictures/frame.png");
        colors.loadPNG("pictures/font2.png");
        //bigcolors.imgResize(x, y);
        sprite bigcolors(colors, 89, 99, 21, 21, 20, 20);
        sprite dots(dot);
        //theta = 0;
        // bigcolors.imgRotate(0);
        //f = 0;int linput;

        int x = 50, y = 50;
        byte alpha=0;
        double size=1;
        double theta=0;
        int f=0;

        clocker tmr_1;
       // rend.set_content_equal_output();
       // rend.set_content_size(300, 300);
        while (running) {

            //input    
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            in.update()
            //simulate
;

            if (in.KEYState(input::key::ESC).down)
                running = false;





            if (tmr_1.millis() > 16)
            {         //render

                //theta -= 0.1;
                rend.fillAll(0x000000ff); //fill opaque black 

                 //rend.drawPoint(, 0xff0000);
                rend.drawLine(0, 0, 299, 299, 0xff0000);
                //rend.drawRect(0, 0, x, y, 0xff0000);
               // rend.drawImage(owl, 0, 0, 0, 0);
              //  bigcolors.imgResize(size);
              //bigcolors.imgRotate(-theta);//20*3.14/180
            //bigcolors.imgHue(0x22ffffff);
                if (in.isKeyDown(input::key::UP))
                    y--;
                if (in.isKeyDown(input::key::LEFT))
                    x--;
                if (in.isKeyDown(input::key::RIGHT))
                    x++;
                if (in.isKeyDown(input::key::DOWN))
                    y++;
                dots.set_frame_number(0);
                dots.resizeNN(3);
                rend.drawSprite(dots, (in.getMousePos(rend.getWWidth(),rend.getWHeight(),rend.getWidth(),rend.getHeight()).x), (in.getMousePos(rend.getWWidth(), rend.getWHeight(), rend.getWidth(), rend.getHeight()).y), 0, 0, 0, 0, alpha);
                bigcolors.set_frame_number(f);
                //bigcolors.resizeNN(size);
                bigcolors.flipH();
                bigcolors.flipV();
                bigcolors.rotate(theta);
                bigcolors.addHue(0xffff0000);

                rend.drawSprite(bigcolors, x, y, 0, 0, 0, 0, alpha);
                rend.drawScreen();
                tmr_1.refresh();
            }




        }

        return 0;
    };
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT result = 0;
        switch (uMsg)
        {
        case WM_KEYDOWN: in.updateKEY(wParam, true); break;
        case WM_KEYUP:  in.updateKEY(wParam, false); break;
        case WM_LBUTTONDOWN:  in.updateKEY(wParam, true); break;
        case WM_LBUTTONUP:	  in.updateKEY(wParam, false); break;
        case WM_RBUTTONDOWN:  in.updateKEY(wParam, true); break;
        case WM_RBUTTONUP:	 in.updateKEY(wParam, false); break;
        case WM_MBUTTONDOWN:  in.updateKEY(wParam, true); break;
        case WM_MBUTTONUP:	 in.updateKEY(wParam, false); break;
        case WM_MOUSEMOVE:  in.updateMousePos(lParam);  break;
        case WM_DESTROY:
        {
            running = FALSE;
        }
        break;
        case WM_PAINT:
        {
            // rend.drawScreen();
        }
        break;
        case WM_SIZE:
        {
            rend.resize();
        }
        break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        }
        return result;
    }


};





/*
    /*int WindowWidth = 800, WindowHeight = 800;

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
    sprite bigcolors(colors,89,99,21,21,20,20);
    theta= 0;
   // bigcolors.imgRotate(0);
    f = 0;
    clocker tmr_1;
    rend.set_content_size(300,300);
    while (running) {
        
        //input    
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
         //simulate

        
 





        if (tmr_1.millis() > 16)
        {         //render
            theta -= 0.1;
            rend.fillAll(0x000000ff); //fill opaque black 

             //rend.drawPoint(, 0xff0000);
            rend.drawLine(0, 0, 299, 299, 0xff0000);
            //rend.drawRect(0, 0, x, y, 0xff0000);
           // rend.drawImage(owl, 0, 0, 0, 0);
          //  bigcolors.imgResize(size);
          //bigcolors.imgRotate(-theta);//20*3.14/180
        //bigcolors.imgHue(0x22ffffff);


            bigcolors.set_frame_number(f);
            //bigcolors.resizeNN(size);
            bigcolors.flipH();
            bigcolors.flipV();
            bigcolors.rotate(theta);
            bigcolors.addHue(0xffff0000);

            rend.drawSprite(bigcolors, x, y, 0, 0, 0, 0, alpha);
            rend.drawScreen();
            tmr_1.refresh();
        } 




    }*/

