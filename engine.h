#ifndef _ENGINE_
#define _ENGINE_

#include <windows.h>
#include "renderer.cpp"
#include "input.cpp"
#include "textfield.cpp"
#include "animation.cpp"
#include "ECS.cpp"
#include "ECScomponents.cpp"




    static std::map<std::string, sprite*> spriteCollection;
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
            createWindow();//it just works
            image dot, owl, testpng, frame, colors, yablochko2;
            MSG msg = { };
            animation test(rend), test2(rend);
            running = true;
            dot.loadBMP("pictures/dot.bmp");
            //owl.loadBMP("pictures/owl.bmp");
            //testpng.loadPNG("pictures/greentransparent.png");
            frame.loadPNG("pictures/snek.png");
            yablochko2.loadPNG("pictures/yablochko.png");
            colors.loadPNG("pictures/x05mb.png");
            
            //bigcolors.imgResize(x, y);
            sprite bigcolors(colors, 20, 20, 0, 0, 0, 0, spriteCollection);
            sprite tests("pictures/yablochko.png",spriteCollection);
            sprite yablochko(yablochko2, spriteCollection);
            textfield text("Hello world", textfield::RW::WRITE, bigcolors, in);
            sprite dots(dot, spriteCollection);
            sprite frame2(frame, 46, 82, 2, 0, 2, 0, spriteCollection);
            test.getAnimationsListFromFile("pictures/test.txt", spriteCollection, "pictures/snek.png");
            test2.getAnimationsListFromFile("pictures/test.txt", spriteCollection, "pictures/snek.png");
            // bigcolors.imgRotate(0);
            //f = 0;int linput;
            ULONGLONG elapsedmillis;
            int x = 50, y = 50;
            byte alpha = 0;
            double size = 0.8;
            double theta = 0;
            int f = 0;
            int applex=0, appley = 0;
            bool draw = false;
            int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            bigcolors.mapAnimation("NONE;!;quot;#;$;%;&;';(;);*;+;,;-;.;/;0;1;2;3;4;5;6;7;8;9;:;smq;<;=;>;?;@;A;B;C;D;E;F;G;H;I;J;K;L;M;N;O;P;Q;R;S;T;U;V;W;X;Y;Z;[;\;];^;_;NN;a;b;c;d;e;f;g;h;i;j;k;l;m;n;o;p;q;r;s;t;u;v;w;x;y;z;{;|;};~;nnnn");
            clocker tmr_1;
            // rend.set_content_equal_output();//to comment agian
            // rend.set_content_size(300, 300);

            std::string str="";
            str = "Elapsed ms: " + std::to_string(tmr_1.millis());
            text.put(str);
            text.hide();
            test.animate("IDLE");
            test2.animate("IDLE");
            test2.position(100, 100);
            ECS tecs;
            entity& snek(tecs.addEntity());  
            snek.addComponent<PositionComponent>(20, 20);
            snek.addComponent<MovementComponent>();
            snek.deleteComponent<MovementComponent>();
            tecs.update();
            while (running) {

                //input    
                PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                //simulate
                elapsedmillis = GetTickCount64();
                if (in.KEYState(input::key::ESC).down)
                    running = false;


                



                if (tmr_1.millis() > 16)
                {         //render
                    in.update(rend.getWWidth(), rend.getWHeight(), rend.getWidth(), rend.getHeight());
                    //theta -= 0.1;
                    rend.fillAll(0x000000ff); //fill opaque black 

                     //rend.drawPoint(, 0xff0000);
                    rend.drawLine(0, 0, 299, 299, 0xff0000);
                  //  rend.fillRect(20, 20, 550, 550, 0xff0000);
                   // rend.drawImage(owl, 0, 0, 0, 0);
                    
                  //bigcolors.imgRotate(-theta);//20*3.14/180
                //bigcolors.imgHue(0x22ffffff);
                    if (in.isKeyDown(input::key::UP))
                    test.animate("IDLE");
                    if (in.isKeyDown(input::key::LEFT))
                        test.animateOnce("AM");
                    if (in.isKeyDown(input::key::RIGHT))
                        test.animateOnce("HE");
                    if (in.isKeyDown(input::key::DOWN))
                        y++;
                    if (in.isKeyPressed(input::key::S))
                        f++;
                    if (in.isKeyPressed(input::key::A))
                        f--;
                    if (in.isKeyPressed(input::key::X))
                        size += 0.1;
                    if (in.isKeyPressed(input::key::Z))
                        size -= 0.1;
                    if (in.isKeyDown(input::key::W))
                        theta += 0.1;
                    if (in.isKeyDown(input::key::Q))
                        theta -= 0.1;
                    if (in.isMBPressed(input::button::LMB))
                    {
                        applex=x1 = in.getMousePos().x, appley=y1 = in.getMousePos().y;

                        draw = false;
                    }
                    if (in.isMBDown(input::button::LMB))
                    {
                        x2 = in.getMousePos().x;
                        y2 = in.getMousePos().y;
                        rend.drawLine(x1, y1, in.getMousePos().x, in.getMousePos().y, 0xFF00FFFF);

                    }
                    if (in.isMBReleased(input::button::LMB))
                    {
                        x2 = in.getMousePos().x;
                        y2 = in.getMousePos().y;
                        draw = true;
                    }
                    if (draw == true)
                        rend.drawLine(x1, y1, x2, y2, 0xFF00FFFF);    
                    text.call();
                    if (in.isKeyPressed(input::key::T))
                    {
                        text.show();
                        text.activate();
                    }



                    //text.setposition(in.getMousePos().x, in.getMousePos().y);
                    text.draw(rend);

                  //  dots.set_frame_number(0);
                  //  dots.resizeNN(3);
                    // rend.drawSprite(dots, in.getMousePos().x, in.getMousePos().y, 0, 0, 0, 0, alpha);
                     //bigcolors.set_frame_number(f);
                   frame2.set_frame_number(f);
                   //frame2.resizeNN(size);
                   yablochko.resizeNN(5);
                 //  frame2.resize(size);//billinear interpolation bug, fix fix fix
                   // bigcolors.resize(size);
                    //  bigcolors.flipH(); yabloko tolika
                    //  bigcolors.flipV();
                   frame2.rotate(theta);
                   rend.drawSprite(frame2, x, y, 0, 0, 0, 0, alpha);
                   //bigcolors.addHue(0xffff0000);
                   
                   rend.drawSprite(yablochko, applex, appley, 0, 0, 0, 0, alpha);
                  // rend.drawSprite(tests, applex, appley, 0, 0, 0, 0, alpha);
                  // rend.drawSprite(bigcolors, x, y, 0, 0, 0, 0, alpha);
                   test.animate(elapsedmillis);
                   test2.animate(elapsedmillis);
                    rend.drawScreen();
                    tmr_1.refresh();
                }




            }
            // MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        };
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            LRESULT result = 0;
            switch (uMsg)
            {
            case WM_KEYDOWN: in.updateKEY(wParam, true); break;
            case WM_KEYUP:  in.updateKEY(wParam, false); break;
            case WM_SYSKEYDOWN: in.updateKEY(wParam, true); break;
            case WM_SYSKEYUP:  in.updateKEY(wParam, false); break;
            case WM_LBUTTONDOWN:  in.updatemKEY(1, true); break;
            case WM_LBUTTONUP:	  in.updatemKEY(1, false); break;
            case WM_RBUTTONDOWN:  in.updatemKEY(2, true); break;
            case WM_RBUTTONUP:	 in.updatemKEY(2, false); break;
            case WM_MBUTTONDOWN:  in.updatemKEY(3, true); break;
            case WM_MBUTTONUP:	 in.updatemKEY(3, false); break;
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
#endif
