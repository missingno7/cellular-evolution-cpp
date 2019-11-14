#include <iostream>
#include <fstream>
#include "cellular-evolution/utilities/bitmap.hpp"

int main() {

    int width=1000;
    int height=1000;
    Bitmap bmp(width,height);

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++) {
            bmp.SetPixel(i,j,i-j,i,j);
        }
    }

    bmp.Write("out.bmp");
    std::cout<<"BMP SAVED"<<std::endl;

    return 0;
}