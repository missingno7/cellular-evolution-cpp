#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/random.h"
#include "cellular-evolution/utilities/bitmap.hpp"


class SaData {
public:
    float *x = nullptr;
    float *y = nullptr;
    int cities_;
    int scWidth_;
    int scHeight_;
    float shiftprob_;
    float revprob_;
    Bitmap bmp_;

    SaData() {

    }

    SaData(PopConfig const &cfg) {
        scWidth_ = cfg.reg.getInt("scwidth")[0];
        scHeight_ = cfg.reg.getInt("scheight")[0];
        shiftprob_ = cfg.reg.getFloat("shiftprob")[0];
        revprob_ = cfg.reg.getFloat("revprob")[0];

        cities_ = cfg.reg.getInt("cities")[0];

        x = new float[cities_];
        y = new float[cities_];

        bmp_.init(scWidth_, scHeight_);

        Random rnd;

        for (int i = 0; i < cities_; i++) {
            x[i] = rnd.nextInt(5, scWidth_ - 5);
            y[i] = rnd.nextInt(5, scHeight_ - 5);

        }
    }

    ~SaData() {
        if (x != nullptr) {
            delete[]x;
            delete[]y;
        }
    }
};
