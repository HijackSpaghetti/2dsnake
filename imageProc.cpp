#include <iostream>
#include <fstream>
#include <windows.h>
#include <math.h>
#include "lib/lodepng.h"




class image {
private:
    void* filedata;// data must contain pixel info in form of RRGGBBAA starting from top left corner
    uint32_t filewidth;
    uint32_t fileheight;
    uint32_t fileimg_size;
    uint32_t transfwidth;
    uint32_t transfheight;
    uint32_t transfimg_size;
    float maxf(float a, float b) { return (a < b) ? a : b; };//find max of two floats
    /*float lerp(float s, float e, float t) { return s + (e - s) * t; }//linear interpolation function
    float blerp(float c00, float c10, float c01, float c11, float tx, float ty) {//bilinear interpoolation function
        return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
    }*/
    byte lerp(byte s, byte e, float t) { return s + (e - s) * t; }//linear interpolation function
    byte blerp(byte c00, byte c10, byte c01, byte c11, float tx, float ty) {//bilinear interpoolation function
        return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
    }
    void sizeRefresh() {
        imgResize(transfwidth, transfheight);
    };


public:
    void* transfdata;// data must contain pixel info in form of RRGGBBAA starting from top left corner
    image() {
        fileheight = 0;
        filewidth = 0;
        fileimg_size = 0;
    };


    int free() {
        fileheight = 0;
        filewidth = 0;
        fileimg_size = 0;
        VirtualFree(filedata, 0, MEM_RELEASE);
        VirtualFree(transfdata, 0, MEM_RELEASE);
        return 0;
    };
   int copy ( image imgS) {
        fileheight = imgS.fileheight;
        filewidth = imgS.filewidth;
        fileimg_size = imgS.fileimg_size;
        filedata = VirtualAlloc(0, fileimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < filewidth * fileheight; xy++)
        {
            ((unsigned int*)filedata)[xy] = ((unsigned int*)imgS.filedata)[xy];
        }
        reset();
        return 0;
    };

    int loadBMP(const char* filename) {//Only supports simple noncompressed BMP files without transparency
        std::ifstream image;
        image.open(filename, std::ios_base::binary);
        std::string string;
        uint16_t sign{ 0 };                         //   signature                      
        uint32_t imgstart{ 0 };                    // 
        uint32_t hsize{ 0 };                      // Size of this header (in bytes)
        uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
        uint16_t bit_count{ 0 };                 // No. of bits per pixel
        uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
        uint32_t size_image{ 0 };                //actual image size in bytes, padding 
        int bytesNotPad, bytesToPad;
        if (image) {
            image.read((char*)&sign, sizeof(sign));
            if (sign != 0x4D42) {
                throw std::runtime_error("Not a BMP format");
            }
            image.seekg(8, image.cur);              //we dont need to read "reserved" bytes
            image.read((char*)&imgstart, sizeof(imgstart));
            image.read((char*)&hsize, sizeof(hsize));
            image.read((char*)&filewidth, sizeof(filewidth));
            image.read((char*)&fileheight, sizeof(fileheight));
            image.read((char*)&planes, sizeof(planes));
            image.read((char*)&bit_count, sizeof(bit_count));
            image.read((char*)&compression, sizeof(compression));
            image.read((char*)&size_image, sizeof(size_image));
            if (compression != 0) {
                throw std::runtime_error("Compressed image not supported");
            }
            int bytesInLine = (filewidth * bit_count) / 8;
            int bytesToRead = bit_count / 8;
            fileimg_size = (filewidth * fileheight * 4);
            bytesNotPad = (filewidth * (bit_count) / 8) % 4;
            bytesToPad = 4 - bytesNotPad;
            void* dataReversed;

            int ioffset;
            dataReversed = VirtualAlloc(0, fileimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            filedata = VirtualAlloc(0, fileimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            image.seekg(imgstart, image.beg);
            for (uint32_t y = 0; y < fileheight; y++) {
                for (uint32_t x = 0; x < filewidth; x++) {
                    ioffset = (y * filewidth * 4) + (x * 4);
                    image.read((char*)dataReversed + (ioffset), bytesToRead);
                }
                if (bytesNotPad != 0)
                    image.seekg(bytesToPad, image.cur);
            }
            for (uint32_t xy = 0; xy < filewidth * fileheight; xy++)
            {
                ((unsigned int*)filedata)[xy] = ((unsigned int*)dataReversed)[filewidth * fileheight - 1 - xy];
                ((char*)filedata)[(xy * 4) + 3] = 0xff;
            }



            VirtualFree(dataReversed, 0, MEM_RELEASE);


        }
        image.close();
        reset();
        return 0;

    };

    int loadPNG(const char* filename) {//PNG handling method, truecolor with alpha type is expected
        std::vector<unsigned char> pngimg; //the raw pixels
        unsigned int lwidth, lheight;


        lodepng::decode(pngimg, lwidth, lheight, filename);

        filewidth = lwidth;
        fileheight = lheight;
        fileimg_size = filewidth * fileheight * 4;

        filedata = VirtualAlloc(0, fileimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        for (uint32_t xy = 0; xy < filewidth * fileheight; xy++)
        {
            ((char*)filedata)[(xy * 4) + 2] = pngimg[(xy * 4)];
            ((char*)filedata)[(xy * 4) + 1] = pngimg[(xy * 4) + 1];
            ((char*)filedata)[(xy * 4)] = pngimg[(xy * 4) + 2];
            ((char*)filedata)[(xy * 4) + 3] = pngimg[(xy * 4) + 3];


        }

        reset();

        return 0;

    };
    int reset() {
        VirtualFree(transfdata, 0, MEM_RELEASE);
        transfdata = VirtualAlloc(0, fileimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        transfheight=fileheight;
        transfwidth=filewidth;
        transfimg_size=fileimg_size;
        int length = filewidth * fileheight;
        for (uint32_t xy = 0; xy < length; xy++)
        {
            ((unsigned int*)transfdata)[xy] = ((unsigned int*)filedata)[xy];
        }

        return 0;
    };
    void* getImage() {
        return transfdata;
    };
    uint32_t getImageHeight() {
        return transfheight;
    };
    uint32_t getImageWidth() {
        return transfwidth;
    };
    uint32_t getImageSize() {
        return transfimg_size;
    };

    uint32_t getFImageWidth() {
        return filewidth;
    };
    uint32_t getFImageHeight() {
        return fileheight;
    };

    uint32_t getFImageSize() {
        return fileimg_size;
    };


    uint32_t getPixel(int x, int y) {
        return (((unsigned int*)filedata)[((y * filewidth) + x)]);
    };

    int imgResize(int newWidth, int newHeight) {


       transfimg_size = newWidth * newHeight * 4;
       transfwidth = newWidth;
       transfheight = newHeight;
       VirtualFree(transfdata, 0, MEM_RELEASE);
        transfdata = VirtualAlloc(0, transfimg_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);






            for (int y = 0; y < newHeight; y++) {
                for (int x = 0; x < newWidth; x++) {


                    float gx = maxf(x / (float)(newWidth) * (filewidth)-0.5f, filewidth - 1);
                    float gy = maxf(y / (float)(newHeight) * (fileheight)-0.5f, fileheight - 1);
                    int gxi = (int)gx;
                    int gyi = (int)gy;
                    float gxd = gx - gxi;
                    float gyd = gy - gyi;
                    uint32_t result = 0;
                    byte c00[4],c10[4],c01[4],c11[4];
                    (*(unsigned int*)&c00) = getPixel(gxi, gyi);
                   
                    (*(unsigned int*)&c10) = getPixel(gxi + 1, gyi);
                   
                   (*(unsigned int*)&c01) = getPixel(gxi, gyi + 1);
                    
                    (*(unsigned int*)&c11) = getPixel(gxi + 1, gyi + 1);
                    
                    result |= blerp(c00[0], c10[0], c01[0], c11[0], gxd, gyd);
                    result |= blerp(c00[1], c10[1], c01[1], c11[1], gxd, gyd) << 8;
                    result |= blerp(c00[2], c10[2], c01[2], c11[2], gxd, gyd) << 16;
                    result |= blerp(c00[3], c10[3], c01[3], c11[3], gxd, gyd) << 24;
                    ((unsigned int*)transfdata)[(y * newWidth) + x] = result;
                }
            }



    



        return 0;
    };
    int imgResize(double mul) {
        int cx = filewidth * mul, cy = fileheight * mul;
        if ((cx != transfwidth) || (cy != transfheight))
         return imgResize(cx, cy);
        
    };
    int imgRotate(double theta) { //theta in degrees
        sizeRefresh();
        void* result;
        double radians = theta;
       // double radians = 3.14159265358979323846 / 180 * theta;
        int xc = transfwidth/2, yc = transfheight/2;//point of rotation
        double sintheta = sin(radians), costheta = cos(radians);//those are invariable for given angle
        double sinmtheta = sin(-1*radians), cosmtheta = cos(-1*radians);
        int xinc, yinc;//calculating new image dimensions
        int bl_x1 = 0, bl_y1 = 0, br_x1= transfwidth, br_y1= 0, tr_x1=transfwidth, tr_y1 = transfheight,tl_x1=0,tl_y1=transfheight;//need to calculate 2 points to find new dimensions
        int br_x2 = (int)(costheta * (br_x1 - xc) - sintheta * (br_y1 - yc) + xc);//calculate side point(only for theta<90deg)
        int tr_y2 = (int)(sintheta * (tr_x1 - xc) + costheta * (tr_y1 - yc) + yc);//top point
        int tl_x2 = (int)(costheta * (tl_x1 - xc) - sintheta * (tl_y1 - yc) + xc);
        int bl_y2 = (int)(sintheta * (bl_x1 - xc) + costheta * (bl_y1 - yc) + yc);
        int tl_y2 = (int)(sintheta * (tl_x1 - xc) + costheta * (tl_y1 - yc) + yc);
        int tr_x2 = (int)(costheta * (tr_x1 - xc) - sintheta * (tr_y1 - yc) + xc);
        int bl_x2 = (int)(costheta * (bl_x1 - xc) - sintheta * (bl_y1 - yc) + xc);
        int br_y2 = (int)(sintheta * (br_x1 - xc) + costheta * (br_y1 - yc) + yc);

        int newwidth = max((abs(tr_x2) + abs(bl_x2)),(abs(br_x2)+abs(tl_x2))), newheight =max((abs(tr_y2)+abs(bl_y2)),(abs(br_y2) + abs(tl_y2)));
        
        int newdimens = newwidth * newheight;
        int result_size = 4 * newdimens;

        result = VirtualAlloc(0, result_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        /*for (uint32_t xy = 0; xy < newdimens; xy++)
        {
            ((unsigned int*)result)[xy] = 0xff000000;
        }*/
        //boudry points (0;0) (transfwidth;-0) (0; transfheight) (transfwidth;transfheight)
        int x1, y1, x2, y2;
        int yca = newwidth/2, xca = newheight/2;
        uint32_t value;
        for (int y = 0; y < newheight; y++) {
            for (int x = 0; x < newwidth; x++) {
                int x2 = (int)(cosmtheta * (x - xca) -sinmtheta * (y - yca)+xc);
                int y2 = (int)(sinmtheta * (x - xca) +cosmtheta * (y - yca)+yc);//where x1,y1 coordinates of untransformed image, starting from center, x2,y2 - where x1y1 value must be
                
                if((0<=x2)&&(x2<transfwidth)&&(0<=y2)&&(y2<transfheight)){ 
                    value = ((unsigned int*)transfdata)[(y2 * transfwidth) + x2];
                ((unsigned int*)result)[((y) * newwidth) + (x)] = value; }
                

                
            }

        }



        VirtualFree(transfdata, 0, MEM_RELEASE);
        transfdata=VirtualAlloc(0, result_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < newdimens; xy++)
        {
            ((unsigned int*)transfdata)[xy] = ((unsigned int*)result)[xy];
        }
        VirtualFree(result, 0, MEM_RELEASE);
        transfwidth = newwidth;
        transfheight = newheight;
        transfimg_size = result_size;
        return 0;
    }
    int refresh() {
    
    };

};