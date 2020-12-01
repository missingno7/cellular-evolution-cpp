#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/cevo/pop_config.h"

class FpData : public IndData {
public:

    int *width;
    int *height;
    int squares;

    ~FpData() {
        delete[]width;
        delete[]height;
    }

    FpData(std::shared_ptr<PopConfig> cfg) {
        squares = cfg->reg.getInt("squares")[0];

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
