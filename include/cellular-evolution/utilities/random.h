#pragma once
//
// Created by jiri on 14/11/19.
//

#include<random>

class Random {

public:

    int nextInt(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        int val = distribution(generator_);
        return val;
    }

    float nextFloat(float min = 0.0, float max = 1.0) {
        std::uniform_real_distribution<float> distribution(min, max);
        float val = distribution(generator_);
        return val;
    }

    std::default_random_engine generator_;

};