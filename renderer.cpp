#include <windows.h>

//LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);






class Renderer {
private:
	HINSTANCE WindowINST;   //the instance of the window
	HWND WindowHandle;      //the handle of the window
	HDC WindowDC;           //Device Context of the window, used for backbuffer
	BITMAPINFO bmi;         //bitmap info struct for StretchDiBits
	void* content;
	int WindowX;            //X pos of the actual window, not the curses window
	int WindowY;            //Y pos of the actual window, not the curses window
	int WindowWidth = 800;        //Width of the actual window, not the curses window
	int WindowHeight = 600;       //Height of the actual window, not the curses window
	

public:
	Renderer() {
        ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = WindowWidth;
        bmi.bmiHeader.biHeight = -WindowHeight;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;   //store it in uncompressed bytes
        bmi.bmiHeader.biSizeImage = WindowWidth * WindowHeight * 1;
        bmi.bmiHeader.biClrUsed = 0;         //the number of colors in our palette

	}
	int init(HINSTANCE hInstance, HWND hwnd, HDC hdc, int w, int h) {
		WindowINST = hInstance;
		WindowHandle = hwnd;
		WindowDC = hdc,
		WindowWidth = w;
		WindowHeight = h;
		bmichange(w,h);
		int buffer_size = WindowWidth * WindowHeight * sizeof(unsigned int);
		if (content)VirtualFree(content, 0, MEM_RELEASE);
		content = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		return 0;
	}
	int bmichange(int w, int h) {
		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = WindowWidth;
		bmi.bmiHeader.biHeight = -WindowHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;   //store it in uncompressed bytes
		bmi.bmiHeader.biSizeImage = WindowWidth * WindowHeight * 1;
		bmi.bmiHeader.biClrUsed = 0;         //the number of colors in our palette
		return 0;
	}
	int resize(int w, int h) {
		WindowWidth = w;
		WindowHeight = h;
		bmichange(w, h);
		int buffer_size = WindowWidth * WindowHeight * sizeof(unsigned int);
		if (content)VirtualFree(content, 0, MEM_RELEASE);
		content = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		return 0;
	};
	int getWidth() {
		return WindowWidth;
	};
	int getHeight() {
		return WindowWidth;
	};
	int drawPoint(int x, int y, unsigned int color) {
		
		if (x < WindowWidth)
		{
			if (y < WindowHeight) {
				((unsigned int*)content)[y*WindowWidth+x] = 0xff0000;
			}
		}
		/*for (y = 0; y < WindowHeight; y++) {
			for (x = 0; x < WindowWidth; x++) {
				((unsigned int*)content)[y * WindowWidth + x] = 0xff0000;
				i++;
			}
		}*/
		return 0;
	};
		
	int drawScreen() {
		StretchDIBits(WindowDC, 0, WindowHeight, WindowWidth, -WindowHeight, 0, 0, WindowWidth, WindowHeight, content, &bmi, DIB_RGB_COLORS, SRCCOPY);
		return 0;
	};

};
