#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/individual.h"
#include "cellular-evolution/cevo/ind_data.h"
#include "test_ind_data.h"
#include  <utility>

class TestInd : public Individual {
public:

    Individual *makeBlank() override {
        return new TestInd();
    }

    void randomize(std::shared_ptr<IndData> &data, Random &rnd) override {
        val_ = rnd.nextInt(-1000, 1000);
        //std::cout<<val_<<std::endl;
    }

    std::shared_ptr<Individual> fromFile(std::string filename) override {
        std::shared_ptr<TestInd> newInd = std::make_shared<TestInd>();
        std::cout << "DUMMY file READ" << std::endl;
        newInd->val_ = 43;
        return newInd;
    }

    void toFile(std::string filename) override {
        std::cout << "DUMMY file WRITE" << std::endl;
    }

    void mutate(float amount, float probability, Random rnd) override {
        val_ += (int) ((rnd.nextFloat(-1000, 1000)) * amount);
    }

    void crossoverTo(Individual *secondOne, Individual *ind, Random &rnd) override {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);
        TestInd *cSecondOne = dynamic_cast<TestInd *>(secondOne);

        float left = rnd.nextFloat();
        float right = 1 - left;

        cInd->val_ = (int) (left * val_ + right * cSecondOne->val_);
    }

    void mutateTo(float amount, float probability, Individual *ind, Random &rnd) override {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);

        int prev = cInd->val_;
        cInd->val_ = val_ + (int) ((rnd.nextFloat(-1000, 1000)) * amount);


        //if(cInd.val<prev)System.out.println(prev+","+cInd.val);
    }

    void copyTo(Individual *ind) override {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);
        cInd->val_ = val_;
        cInd->fitness = fitness;
        cInd->colX = colX;
        cInd->colY = colY;
    }

    void countFitness(std::shared_ptr<IndData> &data) override {
        std::shared_ptr<TestIndData> cData = std::dynamic_pointer_cast<TestIndData>(data);

        fitness = static_cast<int>(val_ * cData->data);
    }

    Individual *clone() override {
        TestInd *newInd = new TestInd();
        newInd->val_ = val_;
        newInd->fitness = fitness;
        newInd->colX = colX;
        newInd->colY = colY;

        return newInd;
    }

    std::string toString(std::shared_ptr<IndData> &data) override {
        std::shared_ptr<TestIndData> cData = std::dynamic_pointer_cast<TestIndData>(data);
        return std::to_string(val_) + " * " + std::to_string(cData->data);
    }

    void countColor() {
        colX = val_ * val_;
        colY = val_;
    }

    int val_;

};
