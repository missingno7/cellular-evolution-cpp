#pragma once
//
// Created by jiri on 05/12/20.
//

#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/random.h"
#include "cellular-evolution/utilities/bitmap.hpp"


class ImgData {
public:


    ImgData() {

    }

    ImgData(PopConfig const &cfg, std::string img_filename) {
        _gt_bmp.loadFile(img_filename);
        _sc_width = _gt_bmp.getWidth();
        _sc_height = _gt_bmp.getHeight();
        _gen_bmp.init(_sc_width, _sc_height);
    }

    ~ImgData() {

    }

    int _sc_width;
    int _sc_height;
    Bitmap _gt_bmp;
    Bitmap _gen_bmp;
};
