#include <vector>
#include <math.h>
#include "image.cpp"


class sprite {
private:


    uint32_t width;//width of non transformed sprite
    uint32_t height;//height of non transformed sprite
   uint32_t sprite_size;// of non transformed sprite
    uint32_t number_of_sprites;//num of sprites in spritesheet
    uint32_t twidth;//width of transformed sprite
    uint32_t theight;//height of transformed sprite
    std::string path;//path to the original image
    double size_mul;
    double angle;
    unsigned int hue;
    bool changed;
    unsigned int current_frame;
    double maxf(float a, float b) { return (a < b) ? a : b; };//find max of two floats
    byte lerp(byte s, byte e, float t) { return s + (e - s) * t; }//linear interpolation function
    byte blerp(byte c00, byte c10, byte c01, byte c11, float tx, float ty) {//bilinear interpoolation function
        return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
    }


    void sizeRefresh() {
        resize(size_mul);
    };
    void refreshSizeRotation() {
        sizeRefresh();
        rotate(angle);
    };


    int resize(int newWidth, int newHeight) {//bilinear interpolation resize
        //todo: needs optimisation


        uint32_t tsize = newWidth * newHeight * 4;
        twidth = newWidth;
        theight = newHeight;
        VirtualFree(buffer, 0, MEM_RELEASE);
        buffer = VirtualAlloc(0, tsize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);






        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {


                float gx = maxf(x / (float)(newWidth) * (width)-0.5f, width - 1);
                float gy = maxf(y / (float)(newHeight) * (height)-0.5f, height - 2);
                int gxi = (int)gx;
                int gyi = (int)gy;
                float gxd = gx - gxi;
                float gyd = gy - gyi;
                uint32_t result = 0;
                byte c00[4], c10[4], c01[4], c11[4];
                (*(unsigned int*)&c00) = ((unsigned int*)spritedata)[(sprite_size*current_frame)+(gyi * width) + gxi];

                (*(unsigned int*)&c10) = ((unsigned int*)spritedata)[(sprite_size * current_frame)+(gyi * width) + gxi + 1];

                (*(unsigned int*)&c01) = ((unsigned int*)spritedata)[(sprite_size * current_frame)+((gyi + 1) * width) + gxi];

                (*(unsigned int*)&c11) = ((unsigned int*)spritedata)[(sprite_size * current_frame)+((gyi + 1) * width) + gxi + 1];

                result |= blerp(c00[0], c10[0], c01[0], c11[0], gxd, gyd);
                result |= blerp(c00[1], c10[1], c01[1], c11[1], gxd, gyd) << 8;
                result |= blerp(c00[2], c10[2], c01[2], c11[2], gxd, gyd) << 16;
                result |= blerp(c00[3], c10[3], c01[3], c11[3], gxd, gyd) << 24;
                ((unsigned int*)buffer)[(y * newWidth) + x] = result;
            }
        }

       
        return 0;
    };

    int resizeNN(int newWidth, int newHeight) {//nearest neighbor resize

        uint32_t tsize = newWidth * newHeight * 4;
        double x_ratio = width / (double)newWidth;
        double y_ratio = height / (double)newHeight;
        VirtualFree(buffer, 0, MEM_RELEASE);
        buffer = VirtualAlloc(0, tsize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        int px, py;
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {


                px = (int)(x * x_ratio);
                py = (int)(y * y_ratio);
                ((unsigned int*)buffer)[(y * newWidth) + x] = ((unsigned int*)spritedata)[(sprite_size * current_frame)+(py * width) + px];
               
            }
        }
        twidth = newWidth;
        theight = newHeight;
      
        return 0;
    };
public:
    void* spritedata;// data must contain pixel info in form of RRGGBBAA starting from top left corner
    void* buffer;

    sprite(image img) {//counting this as singular big sprite
       void* imagedata = img.getImage();
        width = img.getImageWidth();
        height = img.getImageHeight();
        number_of_sprites = 1;
        sprite_size = width * height;
        spritedata=VirtualAlloc(0, (sprite_size*4), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (int xy = 0; xy < width * height; xy++)
        {
            ((unsigned int*)spritedata)[xy] = ((unsigned int*)imagedata)[xy];
        }
        path = img.path;
        size_mul = 1;
        current_frame = 0;
        angle = 0;
        hue = 0x00000000;
    };
    sprite(image img, int fwidth, int fheight, int xdistance, int ydistance,int xoffset, int yoffset) {//cutting image to sprites of given dimension, basically just a rearranging of pixels, x-y-offsets are coordinates of beginning
        void* imagedata = img.getImage();
        width = fwidth;
        height = fheight;
        sprite_size = width * height;
        int sourceheight=img.getImageHeight(), sourcewidth= img.getImageWidth();
        int x_frames = ((img.getImageWidth() - xoffset) / (width + xdistance)), y_frames = ((img.getImageHeight() - yoffset) / (height + ydistance));//calculating how many frames are in given image
        number_of_sprites = x_frames * y_frames;
        int sheetsize = x_frames * y_frames * sprite_size;
        int sheetsizebyte = x_frames * y_frames * sprite_size * 4;
        spritedata = VirtualAlloc(0, sheetsizebyte, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        for (int yf = 0; yf < y_frames; yf++)
        {
            for (int xf = 0; xf < x_frames; xf++)
            {
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        ((unsigned int*)spritedata)[(((yf*x_frames)+xf)*sprite_size)+((y*width)+x)] = ((unsigned int*)imagedata)[((sourcewidth*yoffset)+(yf*(sourcewidth)*(fheight+ydistance))+(y*sourcewidth))+xoffset+(xf*(fwidth+xdistance)+x)];
                    }
                }
            }
        }
        path = img.path;
        size_mul = 1;
        angle = 0;
        hue = 0x00000000;
        current_frame = 0;
    };

    int free() {


        VirtualFree(buffer, 0, MEM_RELEASE);
        VirtualFree(spritedata, 0, MEM_RELEASE);
        return 0;
    };
  

    int refreshdefault(int number) {
    
        VirtualFree(buffer, 0, MEM_RELEASE);
        buffer = VirtualAlloc(0, sprite_size*4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        theight = height;
        twidth = width;
        for (uint32_t xy = 0; xy<sprite_size; xy++)
        {
            ((unsigned int*)buffer)[xy] = ((unsigned int*)spritedata)[(sprite_size * current_frame)+xy];
        }
        return 0;
    };
    void* getSprite() {//this should prepare buffer of transformed sprite of given number and return it. 
        
        return buffer;
    };

    uint32_t getHeight() {
        return theight;
    };
    uint32_t getWidth() {
       return twidth;
    };
    uint32_t getSpriteSize() {
        return twidth*theight;
    };
    int set_frame_number(int number) {
        if (number < number_of_sprites)
            current_frame = number;
        refreshdefault(number);
        return 0;
    };
    int flipV() {
        void* bbuffer;//

        bbuffer = VirtualAlloc(0, twidth*theight*4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < twidth*theight; xy++)
        {
            ((unsigned int*)bbuffer)[xy] = ((unsigned int*)buffer)[xy];
        }
        for (int y = 0; y < theight; y++) {
            for (int x = 0; x < twidth; x++) {


                ((unsigned int*)buffer)[(y * twidth) + x] = ((unsigned int*)bbuffer)[(y*twidth)+(twidth-x)];

            }
        }
        VirtualFree(bbuffer, 0, MEM_RELEASE);
        return 0;
    };

    int flipH() {
        void* bbuffer;//

        bbuffer = VirtualAlloc(0, twidth * theight * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < twidth * theight; xy++)
        {
            ((unsigned int*)bbuffer)[xy] = ((unsigned int*)buffer)[xy];
        }
        for (int y = 0; y < theight; y++) {
            for (int x = 0; x < twidth; x++) {


                ((unsigned int*)buffer)[(y * twidth) + x] = ((unsigned int*)bbuffer)[((theight*twidth)-(y * twidth)) + x];

            }
        }
        VirtualFree(bbuffer, 0, MEM_RELEASE);
        return 0;
    };
    


    int resize(double mul) {
        size_mul = mul;
        int cx = width * mul, cy = height * mul;
        if ((cx != twidth) || (cy != theight))
            return resize(cx, cy);

    };
    int resizeNN(double mul) {
        size_mul = mul;
        int cx = width * mul, cy = height * mul;
        if ((cx != twidth) || (cy != theight))
            return resizeNN(cx, cy);

    };


    int rotate(double theta) { //needs optimisation
        //theta in degrees
        angle = theta;
        if (angle == 0)
            return -1;
        void* result;
        double radians = theta;
        //  double grad = 3.14159265358979323846 / 180 * theta;
        int xc = twidth / 2, yc = theight / 2;//point of rotation
        double sintheta = sin(radians), costheta = cos(radians);//those are invariable for given angle
        //calculating new dimensions
        int bl_x1 = 0, bl_y1 = 0,//input old points, bottom left
            br_x1 = twidth, br_y1 = 0,//bottom right
            tr_x1 = twidth, tr_y1 = theight,//top right
            tl_x1 = 0, tl_y1 = theight;//top left

        int br_x2 = (int)(costheta * (br_x1)-sintheta * (br_y1));//calculating new points, bottom right
        int br_y2 = (int)(sintheta * (br_x1)+costheta * (br_y1));


        int tr_x2 = (int)(costheta * (tr_x1)-sintheta * (tr_y1));
        int tr_y2 = (int)(sintheta * (tr_x1)+costheta * (tr_y1));//top right point


        int tl_x2 = (int)(costheta * (tl_x1)-sintheta * (tl_y1));
        int tl_y2 = (int)(sintheta * (tl_x1)+costheta * (tl_y1));//top left point


        int bl_x2 = (int)(costheta * (bl_x1)-sintheta * (bl_y1));
        int bl_y2 = (int)(sintheta * (bl_x1)+costheta * (bl_y1));//bottom left point






        int newwidth = max((abs(tr_x2) + abs(bl_x2)), (abs(br_x2) + abs(tl_x2))), newheight = (max((abs(tr_y2) + abs(bl_y2)), (abs(br_y2) + abs(tl_y2)))); //calculating new size

        int newdimens = newwidth * newheight;
        int result_size = 4 * newdimens;//x*y*RGBA

        result = VirtualAlloc(0, result_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        int x1, y1, x2, y2;
        int yca = newheight / 2, xca = newwidth / 2;
        uint32_t value;
        for (int y = 0; y < newheight; y++) {
            for (int x = 0; x < newwidth; x++) {
                int x2 = (int)(costheta * (x - xca) - sintheta * (y - yca) + xc);
                int y2 = (int)(sintheta * (x - xca) + costheta * (y - yca) + yc);//where x1,y1 coordinates of untransformed image, starting from center, x2,y2 - where x1y1 value must be

                if ((0 <= x2) && (x2 < twidth) && (0 <= y2) && (y2 < theight)) {

                    value = ((unsigned int*)buffer)[(y2 * twidth) + x2];
                    ((unsigned int*)result)[((y)*newwidth) + (x)] = value;

                }
                // else  ((unsigned int*)result)[((y)*newwidth) + (x)] = 0xff000000;//shows all processed pixels 



            }

        }



        VirtualFree(buffer, 0, MEM_RELEASE);
        buffer = VirtualAlloc(0, result_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < newdimens; xy++)
        {
            ((unsigned int*)buffer)[xy] = ((unsigned int*)result)[xy];
        }
        VirtualFree(result, 0, MEM_RELEASE);
        twidth = newwidth;
        theight = newheight;
        return 0;
    }
    int addHue(unsigned int color) {
       
        hue = color;
        int ybound = theight, xbound = twidth;
        byte colorS[4], colorD[4], colorR[4], remalpha, alpha;//RGBA
        (*(unsigned int*)&colorS) = color;
        for (int iy = 0; iy < ybound; iy++)
        {
            for (int ix = 0; ix < xbound; ix++) {


                (*(unsigned int*)&colorD) = ((unsigned int*)buffer)[((iy)*twidth) + (ix)];
                if (colorD[3] != 0x00) {
                    if (colorS[3] == 0xff)
                        ((unsigned int*)buffer)[((iy)*twidth) + (ix)] = (*(unsigned int*)&colorS);

                    else if (colorS[3] != 0) {


                        alpha = colorS[3];
                        remalpha = 0xff - colorS[3];
                        colorR[0] = ((colorD[0] * remalpha) + (colorS[0] * colorS[3])) >> 8;
                        colorR[1] = ((colorD[1] * remalpha) + (colorS[1] * colorS[3])) >> 8;
                        colorR[2] = ((colorD[2] * remalpha) + (colorS[2] * colorS[3])) >> 8;

                        ((unsigned int*)buffer)[((iy)*twidth) + (ix)] = (*(unsigned int*)&colorR);



                    }
                }
            }

        }

        return 0;
    }



};