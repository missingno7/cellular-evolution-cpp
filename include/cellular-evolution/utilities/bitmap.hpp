#pragma once
////
// Created by jiri on 14/11/19.
//

#include<cassert>

class Bitmap {
public:

    Bitmap(int width, int height)
            : width_(width), height_(height) {
        img = new unsigned char[3 * width * height]();
        const int pad = (4 - (3 * width_) % 4) % 4, filesize = 54 + (3 * width_ + pad) *
                                                                    height_; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes

        for (int i = 29; i < 54; i++) {
            header_[i] = 0;
        }

        for (int i = 0; i < 4; i++) {
            header_[2 + i] = (unsigned char) ((filesize >> (8 * i)) & 255);
            header_[18 + i] = (unsigned char) ((width_ >> (8 * i)) & 255);
            header_[22 + i] = (unsigned char) ((height_ >> (8 * i)) & 255);
        }
    }

    ~Bitmap() {
        delete[] img;
    }

    void clear(unsigned char r, unsigned char g, unsigned char b) {
        for (int i = 0; i < width_; i++) {
            for (int j = 0; j < height_; j++) {
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
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);

        img[3 * (x + y * width_)] = b;
        img[3 * (x + y * width_) + 1] = g;
        img[3 * (x + y * width_) + 2] = r;
    }

    void Write(const std::string path) {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file.write(header_, 54);
        for (int i = height_ - 1; i >= 0; i--) {
            file.write((char *) (img + 3 * width_ * i), 3 * width_);
            file.write(padding_, pad_);
        }
        file.close();
    }

private:

    int width_;
    int height_;
    unsigned char *img;

    // Internal bitmap parameters
    char header_[54] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24,
                        0};
    char padding_[3] = {0, 0, 0};
    int pad_;
};


