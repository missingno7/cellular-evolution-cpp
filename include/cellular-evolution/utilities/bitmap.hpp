#pragma once
////
// Created by jiri on 14/11/19.
//

#include<cassert>

class Bitmap {
public:

    Bitmap() {

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
                SetPixel(i, j, r, g, b);
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
                SetPixel(i, j, r, g, b);
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
            SetPixel(i, y1, r, g, b);
        }

        for (int i = x1; i <= x2; i++) {
            SetPixel(i, y2, r, g, b);
        }

        for (int i = y1; i <= y2; i++) {
            SetPixel(x1, i, r, g, b);
        }

        for (int i = y1; i <= y2; i++) {
            SetPixel(x2, i, r, g, b);
        }
    }

    void drawLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {

        assert(!(x1 == x2 && y1 == y2));

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
                SetPixel(x, y, r, g, b);
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
                SetPixel(x, y, r, g, b);
            }
        }
    }

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);

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


