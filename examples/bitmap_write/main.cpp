#include <iostream>
#include "cellular-evolution/utilities/bitmap.hpp"
#include "cellular-evolution/utilities/random.h"

void DrawCrossBox1(int x1, int y1, int x2, int y2, Bitmap &bmp, int fr=255, int fg=0, int fb=0, int br=255, int bg=255, int bb=0)
{
    bmp.fillRect(x1,y1,x2,y2,br,bg,bb);
    bmp.drawRect(x1,y1,x2,y2,fr, fg, fb);
    bmp.drawLine(x1,y1,x2,y2,fr, fg, fb);
    bmp.drawLine(x1,y2,x2,y1,fr, fg, fb);
}

void DrawCrossBox2(int x1, int y1, int x2, int y2, Bitmap &bmp, int fr=255, int fg=0, int fb=0, int br=255, int bg=255, int bb=0)
{
    bmp.fillRect(x2,y2,x1,y1,br,bg,bb);
    bmp.drawRect(x2,y2,x1,y1,fr, fg, fb);
    bmp.drawLine(x2,y2,x1,y1,fr, fg, fb);
    bmp.drawLine(x2,y1,x1,y2,fr, fg, fb);
}

int main() {

    int width = 256;
    int height = 256;
    Bitmap bmp(width, height);
    Random rnd;


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            bmp.SetPixel(i, j, rnd.nextInt(0, 64), rnd.nextInt(0, 64), rnd.nextInt(0, 64));
        }
    }

    DrawCrossBox1(10,10,110,110,bmp);
    DrawCrossBox2(120,120,220,220,bmp);

    DrawCrossBox1(120,10,220,30,bmp);
    DrawCrossBox2(120,50,220,70,bmp);

    DrawCrossBox1(10,120,30,220,bmp);
    DrawCrossBox1(50,120,70,220,bmp);


    bmp.Write("out.bmp");
    std::cout << "BMP SAVED" << std::endl;

    return 0;
}