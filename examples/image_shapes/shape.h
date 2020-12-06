#pragma once

//
// Created by jiri on 05/12/20.
//

#include "cellular-evolution/utilities/random.h"
#include "img_data.h"

class Shape
        {
        public:

        Shape(Random &rnd, ImgData const &data)
        {
            int x1=rnd.nextInt(10,data._sc_width-10);
            int x2=rnd.nextInt(10,data._sc_width-10);

            if(x1>x2)
            {
                std::swap(x1,x2);
            }

            int y1=rnd.nextInt(10,data._sc_height-10);
            int y2=rnd.nextInt(10,data._sc_height-10);

            if(y1>y2)
            {
                std::swap(y1,y2);
            }


            _xc=(x1+x2)/2;
            _yc=(x1+x2)/2;
            _rx=(x2-x1)/2;
            _ry=(y2-y1)/2;
            _r=rnd.nextInt(0,255);
            _g=rnd.nextInt(0,255);
            _b=rnd.nextInt(0,255);
        }


        private:
        int _xc, _yc, _rx, _ry;
        int _r,_g,_b;
        };