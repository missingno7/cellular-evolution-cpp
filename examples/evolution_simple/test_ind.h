#pragma once
//
// Created by jiri on 14/11/19.
//

#include "test_ind_data.h"
#include  <utility>

class TestInd {
public:

    TestInd(TestIndData const &data)
    {
    }

    TestInd()
    {
    }

    void randomize(TestIndData const &data, Random &rnd) {
        val_ = rnd.nextInt(-1000, 1000);
    }

    TestInd fromFile(std::string filename) {
        TestInd newInd;
        std::cout << "DUMMY file READ" << std::endl;
        newInd.val_ = 43;
        return newInd;
    }

    void toFile(std::string filename) {
        std::cout << "DUMMY file WRITE" << std::endl;
    }

    void mutate(float amount, float probability, Random rnd) {
        val_ += (int) ((rnd.nextFloat(-1000, 1000)) * amount);
    }

    void crossoverTo(TestInd &secondOne, TestInd &ind, Random &rnd, TestIndData const &data) {

        float left = rnd.nextFloat();
        float right = 1 - left;

        ind.val_ = (int) (left * val_ + right * secondOne.val_);
    }

    void mutateTo(float amount, float probability, TestInd &ind, Random &rnd, TestIndData const &data) {
        int prev = ind.val_;
        ind.val_ = val_ + (int) ((rnd.nextFloat(-1000, 1000)) * amount);
    }

    void countFitness(TestIndData const &data) {
        fitness = static_cast<int>(val_ * data.data);
    }

    std::string toString(TestIndData const &data) {
        return std::to_string(val_) + " * " + std::to_string(data.data);
    }

    void countColor(TestIndData const &data) {
        colX = val_ * val_;
        colY = val_;
    }

    float getFitness() {
        return fitness;
    }

    float fitness;
    float colX;
    float colY;

    int val_;
};
