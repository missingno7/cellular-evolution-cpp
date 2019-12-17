#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/random.h"

class SaData : public IndData {
public:
    float *x;
    float *y;
    int cities_;
    int scWidth_;
    int scHeight_;

    SaData(std::shared_ptr<PopConfig> cfg) {
        scWidth_ = cfg->reg.getInt("scwidth")[0];
        scHeight_ = cfg->reg.getInt("scheight")[0];

        cities_ = cfg->reg.getInt("cities")[0];

        x = new float[cities_];
        y = new float[cities_];

        Random rnd;

        for (int i = 0; i < cities_; i++) {
            x[i] = rnd.nextInt(5, scWidth_ - 5);
            y[i] = rnd.nextInt(5, scHeight_ - 5);

        }
    }
};
