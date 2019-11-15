#pragma once
//
// Created by jiri on 14/11/19.
//

#include<random>

class Random {

public:

    Random()
    {
        generator_.seed(seed);
        seed++;
    }

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

    bool nextBoolean()
    {
        return nextInt(0,1);
    }

    std::default_random_engine generator_;

    static int seed;

};
int Random::seed = 0;
