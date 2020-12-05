#pragma once
////
// Created by jiri on 14/11/19.
//

#include<cassert>
#include<fstream>

#define STB_IMAGE_IMPLEMENTATION

#include"stb/stb_image.h"

class Bitmap {
public:

    Bitmap() {

    }

    Bitmap(std::string const bitmap) {
        int width, height, channels;

        unsigned char *res = stbi_load("../data/tst.jpg", &width, &height, &channels, 3);

        assert (res != nullptr);
        init(width, height);

        // Swapping _img for res is not possible because res is in RGB colors, but _img must be in BGR
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                int r = res[3 * (i + j * _width)];
                int g = res[3 * (i + j * _width) + 1];
                int b = res[3 * (i + j * _width) + 2];

                setPixel(i, j, r, g, b);
            }
        }

        delete[]res;
    }


    Bitmap(Bitmap const &bmp) {
        if (bmp._img != nullptr) {
            init(bmp._width, bmp._height);
        }
    }


    Bitmap(int width, int height) {
        init(width, height);
    }

    void init(int width, int height) {
        if (_img != nullptr) {
            delete[] _img;
        }

        _width = width;
        _height = height;
        _img = new unsigned char[3 * width * height]();
        const int pad = (4 - (3 * _width) % 4) % 4, filesize = 54 + (3 * _width + pad) *
                                                                    _height; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes

        for (int i = 29; i < 54; i++) {
            _header[i] = 0;
        }

        for (int i = 0; i < 4; i++) {
            _header[2 + i] = (unsigned char) ((filesize >> (8 * i)) & 255);
            _header[18 + i] = (unsigned char) ((_width >> (8 * i)) & 255);
            _header[22 + i] = (unsigned char) ((_height >> (8 * i)) & 255);
        }
    }

    ~Bitmap() {
        if (_img != nullptr) {
            delete[] _img;
        }
    }

    void clear(unsigned char r, unsigned char g, unsigned char b) {
        for (int i = 0; i < _width; i++) {
            for (int j = 0; j < _height; j++) {
                setPixel(i, j, r, g, b);
            }
        }
    }

    void fillRect(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {
        if (x1 > x2) {
            int tmp = x2;
            x2 = x1;
            x1 = tmp;
        }

        if (y1 > y2) {
            int tmp = y2;
            y2 = y1;
            y1 = tmp;
        }

        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                setPixel(i, j, r, g, b);
            }
        }

    }

    void drawRect(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {

        if (x1 > x2) {
            int tmp = x2;
            x2 = x1;
            x1 = tmp;
        }

        if (y1 > y2) {
            int tmp = y2;
            y2 = y1;
            y1 = tmp;
        }


        for (int i = x1; i <= x2; i++) {
            setPixel(i, y1, r, g, b);
        }

        for (int i = x1; i <= x2; i++) {
            setPixel(i, y2, r, g, b);
        }

        for (int i = y1; i <= y2; i++) {
            setPixel(x1, i, r, g, b);
        }

        for (int i = y1; i <= y2; i++) {
            setPixel(x2, i, r, g, b);
        }
    }

    void drawLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {

        if (x1 == x2 && y1 == y2)return;

        if (abs(x1 - x2) > abs(y1 - y2)) {
            if (x1 > x2) {
                int tmp = x2;
                x2 = x1;
                x1 = tmp;

                tmp = y2;
                y2 = y1;
                y1 = tmp;
            }

            int dx = x2 - x1;
            int dy = y2 - y1;


            for (int x = x1; x <= x2; x++) {
                int y = y1 + dy * (x - x1) / dx;
                setPixel(x, y, r, g, b);
            }

        } else {
            if (y1 > y2) {
                int tmp = x2;
                x2 = x1;
                x1 = tmp;

                tmp = y2;
                y2 = y1;
                y1 = tmp;
            }

            int dx = x2 - x1;
            int dy = y2 - y1;


            for (int y = y1; y <= y2; y++) {
                int x = x1 + dx * (y - y1) / dy;
                setPixel(x, y, r, g, b);
            }
        }
    }

    void drawEllipse(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {
        // Doesn't work correctly for large ellipses
        // bmp.fillEllipse(10,10,442,442,255,255,0);

        int rx = (x2 - x1) / 2;
        int ry = (y2 - y1) / 2;
        int xc = (x2 + x1) / 2;
        int yc = (y2 + y1) / 2;

        //Region 1
        float p = ry * ry - rx * rx * ry + rx * rx / 4.0;
        int x = 0;
        int y = ry;

        while (2.0 * ry * ry * x <= 2.0 * rx * rx * y) {
            if (p < 0) {
                x++;
                p = p + 2 * ry * ry * x + ry * ry;
            } else {
                x++;
                y--;
                p = p + 2 * ry * ry * x - 2 * rx * rx * y - ry * ry;
            }
            setPixel(xc + x, yc + y, 255, 255, 0);
            setPixel(xc + x, yc - y, 255, 255, 0);
            setPixel(xc - x, yc + y, 255, 255, 0);
            setPixel(xc - x, yc - y, 255, 255, 0);
        }

        //Region 2
        p = ry * ry * (x + 0.5) * (x + 0.5) + rx * rx * (y - 1) * (y - 1) - rx * rx * ry * ry;
        while (y > 0) {
            if (p <= 0) {
                x++;
                y--;
                p = p + 2 * ry * ry * x - 2 * rx * rx * y + rx * rx;
            } else {
                y--;
                p = p - 2 * rx * rx * y + rx * rx;
            }
            setPixel(xc + x, yc + y, 255, 255, 0);
            setPixel(xc + x, yc - y, 255, 255, 0);
            setPixel(xc - x, yc + y, 255, 255, 0);
            setPixel(xc - x, yc - y, 255, 255, 0);
        }

    }

    void fillEllipse(int xc, int yc, int rx, int ry, unsigned char r, unsigned char g, unsigned char b) {

        for (int i = 0; i <= rx; i++) {
            for (int j = 0; j <= ry; j++) {

                float p = (pow((i), 2) / pow(rx, 2))
                          + (pow((j), 2) / pow(ry, 2));

                setPixel(xc + i, yc + j, r, g, b);
                setPixel(xc - i, yc + j, r, g, b);
                setPixel(xc + i, yc - j, r, g, b);
                setPixel(xc - i, yc - j, r, g, b);

                if (p > 1) {
                    break;
                }
            }
        }
    }


    void fillEllipseXY(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {
        int xc = (x1 + x2) / 2;
        int yc = (y1 + y2) / 2;
        int rx = (x2 - x1) / 2;
        int ry = (y2 - y1) / 2;
        fillEllipse(xc, yc, rx, ry, r, g, b);
    }


    void fillEllipseHole(int xc, int yc, int rx, int ry,int rxh,int ryh, unsigned char r, unsigned char g, unsigned char b) {

        for (int i = 0; i <= rx; i++) {
            for (int j = 0; j <= ry; j++) {

                float p = (pow((i), 2) / pow(rx, 2))
                          + (pow((j), 2) / pow(ry, 2));


                float p2 = (pow((i), 2) / pow(rxh, 2))
                           + (pow((j), 2) / pow(ryh, 2));


                if (p2>1){
                    setPixel(xc + i, yc + j, r, g, b);
                    setPixel(xc - i, yc + j, r, g, b);
                    setPixel(xc + i, yc - j, r, g, b);
                    setPixel(xc - i, yc - j, r, g, b);
                }

                if (p > 1) {
                    break;
                }
            }
        }
    }


    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
        return;
        }

        _img[3 * (x + y * _width)] = b;
        _img[3 * (x + y * _width) + 1] = g;
        _img[3 * (x + y * _width) + 2] = r;
    }

    void Write(const std::string path) {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(_header, 54);
        for (int i = _height - 1; i >= 0; i--) {
            file.write((char *) (_img + 3 * _width * i), 3 * _width);
            file.write(_padding, _pad);
        }
        file.close();
    }

    inline int getWidth() const {
        return _width;
    }

    inline int getHeight() const {
        return _height;
    }

private:

    int _width = 0;
    int _height = 0;
    unsigned char *_img = nullptr;

    // Internal bitmap parameters
    char _header[54] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24,
                        0};
    char _padding[3] = {0, 0, 0};
    int _pad = 0;
};


