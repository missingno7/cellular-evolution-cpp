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
    int cities_=0;
    int scWidth_;
    int scHeight_;
    float shiftprob_;
    float revprob_;
    Bitmap bmp_;

    SaData() {

    }

    SaData &operator=(const SaData &data) {

        cities_ = data.cities_;

        if (x != nullptr) {
            delete x;
        }
        if (y != nullptr) {
            delete y;
        }
        x = new float[cities_];
        y = new float[cities_];

        scWidth_ = data.scWidth_;
        scHeight_ = data.scHeight_;
        shiftprob_ = data.shiftprob_;
        revprob_ = data.revprob_;

        std::memcpy(x, data.x, cities_ * sizeof(*x));
        std::memcpy(y, data.y, cities_ * sizeof(*y));

        bmp_.init(scWidth_, scHeight_);
    }

    SaData(SaData const &data) {
        cities_ = data.cities_;
        scWidth_ = data.scWidth_;
        scHeight_ = data.scHeight_;
        shiftprob_ = data.shiftprob_;
        revprob_ = data.revprob_;

        x = new float[cities_];
        y = new float[cities_];
        std::memcpy(x, data.x, cities_ * sizeof(*x));
        std::memcpy(y, data.y, cities_ * sizeof(*y));

        bmp_.init(scWidth_, scHeight_);

    }

    SaData(PopConfig const &cfg) {
        scWidth_ = cfg.getInt("scwidth")[0];
        scHeight_ = cfg.getInt("scheight")[0];
        shiftprob_ = cfg.getFloat("shiftprob")[0];
        revprob_ = cfg.getFloat("revprob")[0];

        cities_ = cfg.getInt("cities")[0];

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
