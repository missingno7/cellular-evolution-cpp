#pragma once

//
// Created by jiri on 05/12/20.
//

#include "cellular-evolution/utilities/random.h"
#include "img_data.h"

class Shape {
public:

    Shape(Random &rnd, ImgData const &data) {
        int x1 = rnd.nextInt(10, data._sc_width - 10);
        int x2 = rnd.nextInt(10, data._sc_width - 10);

        if (x1 > x2) {
            std::swap(x1, x2);
        }

        int y1 = rnd.nextInt(10, data._sc_height - 10);
        int y2 = rnd.nextInt(10, data._sc_height - 10);

        if (y1 > y2) {
            std::swap(y1, y2);
        }


        _xc = (x1 + x2) / 2;
        _yc = (y1 + y2) / 2;
        _rx = (x2 - x1) / 2;
        _ry = (y2 - y1) / 2;
        _r = rnd.nextInt(0, 255);
        _g = rnd.nextInt(0, 255);
        _b = rnd.nextInt(0, 255);
    }

    void draw(Bitmap &bmp) const {
        bmp.fillEllipse(_xc, _yc, _rx, _ry, _r, _g, _b);
    }


    void mutate(Random &rnd, ImgData const &data) {
        // Mutate center
        _xc += rnd.nextNormalFloat(0, 10);
        _yc += rnd.nextNormalFloat(0, 10);

        // Mutate R
        _rx += rnd.nextNormalFloat(0, 10);
        _ry += rnd.nextNormalFloat(0, 10);

        if (_rx < 0)_rx = -_rx;
        if (_ry < 0)_ry = -_ry;


        int x1=_xc-_rx;
        int x2=_xc+_rx;
        int y1=_yc-_ry;
        int y2=_yc+_ry;


        if (x1>=data._sc_width-1)
        {
            x1 = data._sc_width-1;
        }
        if (x2>=data._sc_width)
        {
            x2 = data._sc_width;
        }
        if (x1<0)
        {
            x1 = 0;
        }
        if (x2<1)
        {
            x2 = 1;
        }

        if (y1>=data._sc_height-1)
        {
            y1 = data._sc_height-1;
        }
        if (y2>=data._sc_height)
        {
            y2 = data._sc_height;
        }
        if (y1<0)
        {
            y1 = 0;
        }
        if (y2<1)
        {
            y2 = 1;
        }

        _xc = (x1 + x2) / 2;
        _yc = (y1 + y2) / 2;
        _rx = (x2 - x1) / 2;
        _ry = (y2 - y1) / 2;

        // Mutate color
        _r += rnd.nextNormalFloat(0, 10);
        _g += rnd.nextNormalFloat(0, 10);
        _b += rnd.nextNormalFloat(0, 10);

        if (_r > 255)_r = 255;
        if (_g > 255)_g = 255;
        if (_b > 255)_b = 255;
        if (_r < 0)_r = 0;
        if (_g < 0)_g = 0;
        if (_b < 0)_b = 0;


    }

    void countColor(float &x, float &y) const {
        x += _r - 128;
        x += _g - 128;
        x += _b - 128;
        y += _xc - _yc;
        y += _rx - _ry;
    }


private:
    int _xc, _yc, _rx, _ry;
    int _r, _g, _b;
};