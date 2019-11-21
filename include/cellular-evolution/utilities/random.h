#pragma once
//
// Created by jiri on 14/11/19.
//

#include<random>
#include <time.h>

class Random {

public:

    Random()
    {
        generator_.seed(time(NULL)+seed_);
        seed_++;
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

    float nextNormalFloat(float mean = 0.0, float std_dev = 1.0) {
        std::normal_distribution<float> distribution(mean, std_dev);
        float val = distribution(generator_);
        return val;
    }


    bool nextBoolean()
    {
        return nextInt(0,1);
    }

    std::default_random_engine generator_;

    static uint64_t seed_;

};
uint64_t Random::seed_ = 0;
