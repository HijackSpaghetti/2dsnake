#include <windows.h>

//LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);






class Renderer {
private:
	HINSTANCE WindowINST;   //the instance of the window
	HWND WindowHandle;      //the handle of the window
	HDC WindowDC;           //Device Context of the window, used for backbuffer
	BITMAPINFO bmi;         //bitmap info struct for StretchDiBits
	WINDOWINFO wi;
	void* content;
	int WindowX;            //X pos of the actual window, not the curses window
	int WindowY;            //Y pos of the actual window, not the curses window
	int windowTBorder;		//size of top border
	int windowBBorder;		//size of bottom border
	int windowLBorder;		//size of left border
	int windowRBorder;		//size of right border
	int WindowWidth = 800;        //Width of the actual window, not the curses window
	int WindowHeight = 600;       //Height of the actual window, not the curses window
	

public:
	Renderer() {

	}
	int init(HINSTANCE hInstance, HWND hwnd, HDC hdc) {
		WindowINST = hInstance;
		WindowHandle = hwnd;
		WindowDC = hdc,
		resize();
		return 0;
	}
	int bmiupdate() {
		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = WindowWidth;
		bmi.bmiHeader.biHeight = WindowHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;   //store it in uncompressed bytes
		//bmi.bmiHeader.biSizeImage = WindowWidth * WindowHeight * 1;
		//bmi.bmiHeader.biClrUsed = 0;         //the number of colors in our palette
		return 0;
	}
	int resize() {
		wi.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(WindowHandle, &wi);
		RECT crect;
		GetClientRect(WindowHandle, &crect);
		WindowWidth = (wi.rcClient.right-wi.rcClient.left);
		WindowHeight = (wi.rcClient.bottom-wi.rcClient.top);
		windowTBorder=wi.rcClient.top-wi.rcWindow.top;		//size of top border
		windowBBorder=wi.rcWindow.bottom-wi.rcClient.bottom;		//size of bottom border
		windowLBorder=wi.rcClient.left-wi.rcWindow.left;		//size of left border
		windowRBorder=wi.rcWindow.right-wi.rcClient.right;		//size of right border


		bmiupdate();
		int buffer_size = WindowWidth * WindowHeight * sizeof(unsigned int);
		if (content)VirtualFree(content, 0, MEM_RELEASE);
		content = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		return 0;
	};
	int getWidth() {
		return WindowWidth;
	};
	int getHeight() {
		return WindowHeight;
	};
	int drawLine(int x1, int y1, int x2, int y2, int color) {
		
		double m,b;
		int y,yi;
		if (x1 == x2) {
			for (int y = y1; y < y2; y++) {

				((unsigned int*)content)[y * WindowWidth + x1] = color;
			}
		}

			if (x1 != x2) {
				m = (double)(y2 - y1) / (x2 - x1);
				b = (double)y1 - (m * x1);
				for (int x = x1; x <= x2; x++) {
					y = -1;
					y = m * x + b;
					if (x < x2)
						yi = m * (x + 1) + b;
					for (int i = y; i < yi; i++)
						((unsigned int*)content)[i * WindowWidth + x] = color;

					((unsigned int*)content)[y * WindowWidth + x] = color;
				}

			}

			return 0;
	};


	int drawRect(int x1, int y1, int x2, int y2, int color) {
		
		for (int y = y1; y < y2; y++) {

			((unsigned int*)content)[y * WindowWidth + x1] = color;
			((unsigned int*)content)[y * WindowWidth + x2] = color;
		}
		for (int x = x1; x < x2; x++) {

			((unsigned int*)content)[y1 * WindowWidth + x] = color;
			((unsigned int*)content)[y2 * WindowWidth + x] = color;
		}
		return 0;

	};

	int fillAll(unsigned int color) {

		for (int y = 0; y < WindowHeight; y++) {
			for (int x = 0; x <= WindowWidth; x++) {
				((unsigned int*)content)[y * WindowWidth + x] = color;
			}
		}
		return 0;
	};
	
	int drawPoint(int x, int y, unsigned int color) {

		if (y < WindowHeight)
		{
			if (x < WindowWidth) {
				((unsigned int*)content)[y*WindowWidth+x] = color;
			}
		}

		return 0;
	};
	//int findBorders() {}
	int drawScreen() {
		StretchDIBits(WindowDC, windowLBorder, WindowHeight+windowTBorder-1, WindowWidth,-WindowHeight, 0, 0, WindowWidth, WindowHeight, content, &bmi, DIB_RGB_COLORS, SRCCOPY);
		return 0;
	};

};
