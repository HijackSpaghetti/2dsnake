#include <iostream>
#include <fstream>
#include <windows.h>
#include "lib/lodepng.h"




class image {
private:
    uint32_t width;
    uint32_t height;
    uint32_t img_size;



  
   


public:
    void* data;// data must contain pixel info in form of RRGGBBAA starting from top left corner
    std::string path;
    image() {
        height = 0;
        width = 0;
        img_size = 0;
    };


    int free() {
        height = 0;
        width = 0;
        img_size = 0;

        VirtualFree(data, 0, MEM_RELEASE);
        return 0;
    };
   int copy ( image imgS) {//probably never called
        height = imgS.height;
        width = imgS.width;
        img_size = imgS.img_size;

        data = VirtualAlloc(0, img_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        for (uint32_t xy = 0; xy < width * height; xy++)
        {
            ((unsigned int*)data)[xy] = ((unsigned int*)imgS.data)[xy];
        }

        return 0;
    };

    int loadBMP(const char* filename) {//Only supports simple noncompressed BMP files without transparency
        path = filename;
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
            image.read((char*)&width, sizeof(width));
            image.read((char*)&height, sizeof(height));
            image.read((char*)&planes, sizeof(planes));
            image.read((char*)&bit_count, sizeof(bit_count));
            image.read((char*)&compression, sizeof(compression));
            image.read((char*)&size_image, sizeof(size_image));
            if (compression != 0) {
                throw std::runtime_error("Compressed image not supported");
            }
            int bytesInLine = (width * bit_count) / 8;
            int bytesToRead = bit_count / 8;
            img_size = (width * height * 4);
            bytesNotPad = (width * (bit_count) / 8) % 4;
            bytesToPad = 4 - bytesNotPad;
            void* dataReversed;

            int ioffset;
            dataReversed = VirtualAlloc(0, img_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            data = VirtualAlloc(0, img_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            image.seekg(imgstart, image.beg);
            for (uint32_t y = 0; y < height; y++) {
                for (uint32_t x = 0; x < width; x++) {
                    ioffset = (y * width * 4) + (x * 4);
                    image.read((char*)dataReversed + (ioffset), bytesToRead);
                }
                if (bytesNotPad != 0)
                    image.seekg(bytesToPad, image.cur);
            }
            for (uint32_t xy = 0; xy < width * height; xy++)
            {
                ((unsigned int*)data)[xy] = ((unsigned int*)dataReversed)[width * height - 1 - xy];
                ((char*)data)[(xy * 4) + 3] = 0xff;
            }



            VirtualFree(dataReversed, 0, MEM_RELEASE);


        }
        image.close();

        return 0;

    };

    int loadPNG(const char* filename) {//PNG handling method, truecolor with alpha type is expected
        std::vector<unsigned char> pngimg; //the raw pixels
        unsigned int lwidth, lheight;
        path = filename;

        lodepng::decode(pngimg, lwidth, lheight, filename);

        width = lwidth;
        height = lheight;
        img_size = width * height * 4;

        data = VirtualAlloc(0, img_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        for (uint32_t xy = 0; xy < width * height; xy++)
        {
            ((char*)data)[(xy * 4) + 2] = pngimg[(xy * 4)];
            ((char*)data)[(xy * 4) + 1] = pngimg[(xy * 4) + 1];
            ((char*)data)[(xy * 4)] = pngimg[(xy * 4) + 2];
            ((char*)data)[(xy * 4) + 3] = pngimg[(xy * 4) + 3];


        }



        return 0;

    };
  
    void* getImage() {
        return data;
    }

    uint32_t getImageWidth() {
        return width;
    };
    uint32_t getImageHeight() {
        return height;
    };

    uint32_t getImageSize() {
        return img_size;
    };






    



};