#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/bitmap.hpp"


class FpData {
public:

    int *width = nullptr;
    int *height = nullptr;
    int squares = 0;
    int scWidth;
    int scHeight;
    float flip_prob_;
    float switch_prob_;
    Bitmap bmp_;

    FpData() {

    }

    FpData &operator=(const FpData &data) {
        scWidth = data.scWidth;
        scHeight = data.scHeight;

        flip_prob_ = data.flip_prob_;
        switch_prob_ = data.switch_prob_;

        bmp_.init(scWidth, scHeight);

        if (squares != data.squares) {
            if (width != nullptr) {
                delete[]width;
            }
            if (height != nullptr) {
                delete[]height;
            }

            squares = data.squares;
            width = new int[squares];
            height = new int[squares];
        }

        for (int i = 0; i < squares; i++) {
            width[i] = data.width[i];
            height[i] = data.height[i];
        }

        return *this;
    }

    FpData(FpData const &data) {
        squares = data.squares;

        scWidth = data.scWidth;
        scHeight = data.scHeight;

        flip_prob_ = data.flip_prob_;
        switch_prob_ = data.switch_prob_;

        bmp_.init(scWidth, scHeight);

        width = new int[squares];
        height = new int[squares];

        for (int i = 0; i < squares; i++) {
            width[i] = data.width[i];
            height[i] = data.height[i];
        }
    }

    ~FpData() {
        delete[]width;
        delete[]height;
    }

    FpData(PopConfig const &cfg) {
        squares = cfg.getInt("squares")[0];

        scWidth = 1000;
        scHeight = 1000;

        flip_prob_ = cfg.getFloat("flipprob")[0];
        switch_prob_ = cfg.getFloat("switchprob")[0];

        bmp_.init(scWidth, scHeight);

        width = new int[squares];
        height = new int[squares];
        Random rnd;

        for (int i = 0; i < squares; i++) {
            width[i] = 10 + std::abs(rnd.nextInt(0, 100));
            height[i] = 10 + std::abs(rnd.nextInt(0, 100));
        }
    }

    int getPerfection() {
        int sum = 0;
        for (int i = 0; i < squares; i++) {
            sum += width[i] * height[i];
        }

        return sum;
    }
};
