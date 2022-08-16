#include <windows.h>
#include "sprite.cpp"
#include "clocker.cpp"







class Renderer {
private:
	HINSTANCE WindowINST;   //the instance of the window
	HWND WindowHandle;      //the handle of the window
	HDC WindowDC;           //Device Context of the window, used for drawing
	BITMAPINFO bmi;         //bitmap info struct for StretchDiBits
	WINDOWINFO wi;			//windows info struct
	void* content;//finalised content for StretchDiBit
	void* output;
	int cwidth=300;
	int cheight=300;
	bool content_equal_output = false;
	int WindowX;            //X pos of the actual window, not the curses window
	int WindowY;            //Y pos of the actual window, not the curses window
	int windowTBorder;		//size of top border
	int windowBBorder;		//size of bottom border
	int windowLBorder;		//size of left border
	int windowRBorder;		//size of right border
	int WindowWidth = 800;        //Width of the actual client area
	int WindowHeight = 600;       //Height of the actual client area
	image imagebuffer;			//placeholder for images to transform on the fly

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
		if (content_equal_output == true) {
			cwidth = WindowWidth;
			cheight = WindowHeight;
		}

		if (content)VirtualFree(content, 0, MEM_RELEASE);
		content = VirtualAlloc(0, cwidth * cheight * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		return 0;
	};
	int getWidth() {
		return cwidth;
	};
	int getHeight() {
		return cheight;
	};
	int getWWidth() {
		return WindowWidth;
	};
	int getWHeight() {
		return WindowHeight;
	};
	int set_content_equal_output(){
		content_equal_output = true;
		resize();
		return 0;
	}
	int set_content_size(int contentwidth, int contentheight) {
		content_equal_output = false;
		cwidth = contentwidth;
		cheight = contentheight;
		resize();
		return 0;
	}
	int stretchContent() {//nearest neigbour content stretching for output
		
		uint32_t tsize = WindowWidth * WindowHeight * sizeof(unsigned int);
		double x_ratio = cwidth / (double)WindowWidth;
		double y_ratio = cheight / (double)WindowHeight;
		VirtualFree(output, 0, MEM_RELEASE);
		output = VirtualAlloc(0, tsize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		int px, py;
		for (int y = 0; y < WindowHeight; y++) {
			for (int x = 0; x < WindowWidth; x++) {


				px = (int)(x * x_ratio);
				py = (int)(y * y_ratio);
				((unsigned int*)output)[(y * WindowWidth) + x] = ((unsigned int*)content)[ + (py * cwidth) + px];

			}
		}


		return 0;
	
	};

	int drawScreen() {
		stretchContent();
		StretchDIBits(WindowDC, windowLBorder, WindowHeight + windowTBorder - 1, WindowWidth, -WindowHeight, 0, 0, WindowWidth, WindowHeight, output, &bmi, DIB_RGB_COLORS, SRCCOPY);
		return 0;
	};
	int drawLine(int x1, int y1, int x2, int y2, int color) {
		
		double m,b;
		int y,yi;
		if (x1 == x2) {
			for (int y = y1; y < y2; y++) {

				((unsigned int*)content)[y * cwidth + x1] = color;
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
						((unsigned int*)content)[i * cwidth + x] = color;

					((unsigned int*)content)[y * cwidth + x] = color;
				}

			}

			return 0;
	};


	int drawRect(int x1, int y1, int x2, int y2, int color) {
		
		for (int y = y1; y < y2; y++) {

			((unsigned int*)content)[y * cwidth + x1] = color;
			((unsigned int*)content)[y * cwidth + x2] = color;
		}
		for (int x = x1; x < x2; x++) {

			((unsigned int*)content)[y1 *cwidth + x] = color;
			((unsigned int*)content)[y2 * cwidth + x] = color;
		}
		return 0;

	};
	int drawImage(sprite img, int x1, int y1, int x2, int y2) {
		void* im;
		im = img.getSprite();
		int imwidth = img.getWidth(); 
		int imheight = img.getHeight();
		int xbound = imwidth;
		int ybound = imheight;
		int xstart = 0, ystart = 0;
		if (x1 < 0)
			xstart = xstart - x1;
		if (y1 < 0)
			ystart = ystart - y1;
		if (imwidth + x1 > cwidth)
			xbound = (cwidth - x1);
		if (imheight + y1 > cheight)
			ybound = cheight - y1;
		//300 pixel display, 200p x offset, 150p, pixels to draw 100. 
		
		for (int iy=ystart; iy < ybound; iy++)
		{
			for (int ix=xstart; ix < xbound; ix++) {
			((unsigned int*)content)[((iy+y1)*cwidth) + (ix+x1)] = ((unsigned int*)im)[iy * imwidth + ix];


			}
			

		}
		return 0;

	};
	int drawImageA(sprite img, int xdest, int ydest, int xsource, int ysource, int sourcewidth,int sourceheight, byte subalpha) {
		void* im;
		im = img.getSprite();
		int imwidth = img.getWidth();
		int imheight = img.getHeight();
		if (sourcewidth+xsource > imwidth)
			return -1;
		if (sourceheight+ysource > imheight)
			return -1;

		int xbound = imwidth;
		int ybound = imheight;
		int xstart = 0, ystart = 0;
		byte colorS[4], colorD[4], colorR[4];//RGBA

		byte remalpha,alpha;
		if (xdest < 0)
			xstart = xstart - xdest;
		if (ydest < 0)
			ystart = ystart - ydest;

		if (imwidth + xdest > WindowWidth)
			xbound = (WindowWidth - xdest);
		

		if (imheight + ydest > WindowHeight)
			ybound = WindowHeight - ydest;
		if (sourcewidth!=0) 
			if (xbound > sourcewidth)
				xbound = sourcewidth;
		if (sourceheight != 0)
			if (ybound > sourceheight)
				ybound = sourceheight;
	

		for (int iy = ystart; iy < ybound; iy++)
		{
			for (int ix = xstart; ix < xbound; ix++) {

				(*(unsigned int*)&colorS) = ((unsigned int*)im)[((iy)*imwidth)+ (ix)];//bugging
				if (subalpha != 0){
					if (subalpha >= colorS[3]) {
						colorS[3] = 0;
					}
					else colorS[3] -= subalpha;
				}

				
				if (colorS[3] == 0xff)
					((unsigned int*)content)[((iy + ydest) * WindowWidth) + (ix + xdest)] = (*(unsigned int*)&colorS);

				else if (colorS[3]!= 0) {
		
					(*(unsigned int*)&colorD) = ((unsigned int*)content)[((iy + ydest) * WindowWidth) + (ix + xdest)];
					alpha = colorS[3];
					remalpha = 0xff - colorS[3];
					colorR[0] = ((colorD[0] * remalpha) + (colorS[0] * colorS[3])) >> 8;
					colorR[1] = ((colorD[1] * remalpha) + (colorS[1] * colorS[3])) >> 8;
					colorR[2] = ((colorD[2] * remalpha) + (colorS[2] * colorS[3])) >> 8;
					
					((unsigned int*)content)[((iy + ydest) * WindowWidth) + (ix + xdest)] = (*(unsigned int*)&colorR);

					
				}
			}


		}
		return 0;

	};
	int drawSpriteA(sprite img, int xdest, int ydest, int xsource, int ysource, int sourcewidth, int sourceheight, byte subalpha) {
		void* im;
		im = img.getSprite();
		int imwidth = img.getWidth();
		int imheight = img.getHeight();
		if (sourcewidth + xsource > imwidth)
			return -1;
		if (sourceheight + ysource > imheight)
			return -1;

		int xbound = imwidth;
		int ybound = imheight;
		int xstart = 0, ystart = 0;
		byte colorS[4], colorD[4], colorR[4];//RGBA

		byte remalpha, alpha;
		if (xdest < 0)
			xstart = xstart - xdest;
		if (ydest < 0)
			ystart = ystart - ydest;

		if (imwidth + xdest > cwidth)
			xbound = (cwidth - xdest);


		if (imheight + ydest > cheight)
			ybound = cheight - ydest;
		if (sourcewidth != 0)
			if (xbound > sourcewidth)
				xbound = sourcewidth;
		if (sourceheight != 0)
			if (ybound > sourceheight)
				ybound = sourceheight;


		for (int iy = ystart; iy < ybound; iy++)
		{
			for (int ix = xstart; ix < xbound; ix++) {

				(*(unsigned int*)&colorS) = ((unsigned int*)im)[((iy)*imwidth) + (ix)];
				if (subalpha != 0) {
					if (subalpha >= colorS[3]) {
						colorS[3] = 0;
					}
					else colorS[3] -= subalpha;
				}


				if (colorS[3] == 0xff)
					((unsigned int*)content)[((iy + ydest) * cwidth) + (ix + xdest)] = (*(unsigned int*)&colorS);

				else if (colorS[3] != 0) {

					(*(unsigned int*)&colorD) = ((unsigned int*)content)[((iy + ydest) * cwidth) + (ix + xdest)];
					alpha = colorS[3];
					remalpha = 0xff - colorS[3];
					colorR[0] = ((colorD[0] * remalpha) + (colorS[0] * colorS[3])) >> 8;
					colorR[1] = ((colorD[1] * remalpha) + (colorS[1] * colorS[3])) >> 8;
					colorR[2] = ((colorD[2] * remalpha) + (colorS[2] * colorS[3])) >> 8;

					((unsigned int*)content)[((iy + ydest) * cwidth) + (ix + xdest)] = (*(unsigned int*)&colorR);


				}
			}


		}
		return 0;

	};


	int drawImageFrame(image img, int xdest, int ydest, int framewidth, int frameheight, int frameXpos, int frameYpos, byte subalpha) {
		
			int sourceX=framewidth*frameXpos;
			int sourceY=frameheight*frameYpos;
			int e = 0;
			if ((e = drawImageA(img, xdest, ydest, sourceX, sourceY, framewidth, frameheight, subalpha)) != 0)
				return e;
			

		return 0;

	};
	int drawImageCenteredFrame(image img, int xdest, int ydest, int framewidth, int frameheight, int frameXpos, int frameYpos, byte subalpha) {//centered

		int sourceX = framewidth * frameXpos;
		int sourceY = frameheight * frameYpos;
		int nxdest = xdest-(img.getImageWidth() / 2);
		int nydest = ydest - (img.getImageHeight() / 2);
		int e = 0;
		if ((e = drawImageA(img, nxdest, nydest, sourceX, sourceY, framewidth, frameheight, subalpha)) != 0)
			return e;


		return 0;

	};


	int drawSprite(sprite img, int xdest, int ydest, int xsource, int ysource, int sourcewidth, int sourceheight, byte subalpha) {//centered


		int nxdest = xdest - (img.getWidth() / 2);
		int nydest = ydest - (img.getHeight() / 2);
		int e = 0;
		if ((e = drawSpriteA(img, nxdest, nydest, xsource, ysource, sourcewidth, sourceheight, subalpha)) != 0)
			return e;


		return 0;

	};

	int fillAll(unsigned int color) {

		for (int y = 0; y < cheight; y++) {
			for (int x = 0; x <= cwidth; x++) {
				((unsigned int*)content)[y * cwidth+ x] = color;
			}
		}
		return 0;
	};
	
	int drawPoint(int x, int y, unsigned int color) {

		if (y < WindowHeight)
		{
			if (x < WindowWidth) {
				((unsigned int*)content)[y*cwidth+x] = color;
			}
		}

		return 0;
	};


};
