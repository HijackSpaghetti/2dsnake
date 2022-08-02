#ifndef UNICODE
#define UNICODE
#endif 
#include "window.h"

BOOL running;



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    GameWindow gWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);


 
    if (gWindow.getHWND() == NULL)
        return 0;
    running = TRUE;

    MSG msg = { };
    while (running) {

        //input    
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
         //simulate



         //render
                 

        

    }



    return 0;
}

