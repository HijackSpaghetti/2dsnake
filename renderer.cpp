#include <windows.h>
#include "imageProc.cpp"
//LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);






class Renderer {
private:
	HINSTANCE WindowINST;   //the instance of the window
	HWND WindowHandle;      //the handle of the window
	HDC WindowDC;           //Device Context of the window, used for drawing
	BITMAPINFO bmi;         //bitmap info struct for StretchDiBits
	WINDOWINFO wi;			//windows info struct
	void* content;			//finalised content for StretchDiBits
	void* acontent;			//content with alphachannel
	int WindowX;            //X pos of the actual window, not the curses window
	int WindowY;            //Y pos of the actual window, not the curses window
	int windowTBorder;		//size of top border
	int windowBBorder;		//size of bottom border
	int windowLBorder;		//size of left border
	int windowRBorder;		//size of right border
	int WindowWidth = 800;        //Width of the actual client area
	int WindowHeight = 600;       //Height of the actual client area
	

public:
	Renderer() {
		//initialisation is in init();
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

		return 0;
	}
	int resize() {
		wi.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(WindowHandle, &wi);
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
	int drawScreen() {
		StretchDIBits(WindowDC, windowLBorder, WindowHeight + windowTBorder - 1, WindowWidth, -WindowHeight, 0, 0, WindowWidth, WindowHeight, content, &bmi, DIB_RGB_COLORS, SRCCOPY);
		return 0;
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
				m = (double)(y2 - y1) / (x2 - x1);		//simple arithmetics, straightforward int to double casting and back, feeling bald Dx
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
	int drawImage(image img, int x1, int y1, int x2, int y2) {
		void* im;
		im = img.getImage();
		int imwidth = img.getImageWidth(); 
		int imheight = img.getImageHeight();
		int xbound = imwidth;
		int ybound = imheight;
		int xstart = 0, ystart = 0;
		if (x1 < 0)
			xstart = xstart - x1;
		if (y1 < 0)
			ystart = ystart - y1;
		if (imwidth + x1 > WindowWidth)
			xbound = (WindowWidth - x1);
		if (imheight + y1 > WindowHeight)
			ybound = WindowHeight - y1;
		//300 pixel display, 200p x offset, 150p, pixels to draw 100. 
		
		for (int iy=ystart; iy < ybound; iy++)
		{
			for (int ix=xstart; ix < xbound; ix++) {
			((unsigned int*)content)[((iy+y1)*WindowWidth) + (ix+x1)] = ((unsigned int*)im)[iy * imwidth + ix];


			}
			

		}
		return 0;

	};
	int drawAlphaImage(image img, int x1, int y1, int x2, int y2) {
		void* im;
		im = img.getImage();
		int imwidth = img.getImageWidth();
		int imheight = img.getImageHeight();
		int xbound = imwidth;
		int ybound = imheight;
		int xstart = 0, ystart = 0;
		if (x1 < 0)
			xstart = xstart - x1;
		if (y1 < 0)
			ystart = ystart - y1;
		if (imwidth + x1 > WindowWidth)
			xbound = (WindowWidth - x1);
		if (imheight + y1 > WindowHeight)
			ybound = WindowHeight - y1;
		//300 pixel display, 200p x offset, 150p, pixels to draw 100. 

		for (int iy = ystart; iy < ybound; iy++)
		{
			for (int ix = xstart; ix < xbound; ix++) {
				((unsigned int*)content)[((iy + y1) * WindowWidth) + (ix + x1)] = ((unsigned int*)im)[iy * imwidth + ix];


			}


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


};
