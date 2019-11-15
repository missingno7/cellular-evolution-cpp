#include <iostream>
#include <fstream>
#include "cellular-evolution/utilities/bitmap.hpp"
#include "cellular-evolution/utilities/random.h"

int main() {

    int width = 1000;
    int height = 1000;
    Bitmap bmp(width, height);
    Random rnd;


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            bmp.SetPixel(i, j, rnd.nextInt(0, 255), rnd.nextInt(0, 255), rnd.nextInt(0, 255));
        }
    }

    bmp.Write("out.bmp");
    std::cout << "BMP SAVED" << std::endl;

    return 0;
}