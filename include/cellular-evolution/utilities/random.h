#pragma once
//
// Created by jiri on 14/11/19.
//

#include<random>
#include <time.h>
#include <mutex>


class Random {

public:

    Random()
    {
        {
            std::lock_guard<std::mutex> l(seed_mutex_);
            seed_+=time(NULL);
            generator_.seed(seed_);
        }
    }

    int nextInt(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        int val;
        {
            //std::lock_guard<std::mutex> l(seed_mutex_);
        val = distribution(generator_);
        }
        return val;
    }

    float nextFloat(float min = 0.0, float max = 1.0) {
        std::uniform_real_distribution<float> distribution(min, max);
        float val;
        {
            //std::lock_guard<std::mutex> l(seed_mutex_);
            val = distribution(generator_);
        }
        return val;
    }

    float nextNormalFloat(float mean = 0.0, float std_dev = 1.0) {
        std::normal_distribution<float> distribution(mean, std_dev);
        float val;
        {
            //std::lock_guard<std::mutex> l(seed_mutex_);
        val = distribution(generator_);
        }
        return val;
    }


    bool nextBoolean()
    {
        return nextInt(0,1);
    }

    std::default_random_engine generator_;

    static uint64_t seed_;

    static std::mutex seed_mutex_;


};
uint64_t Random::seed_=0;
std::mutex Random::seed_mutex_;
//std::default_random_engine Random::generator_;