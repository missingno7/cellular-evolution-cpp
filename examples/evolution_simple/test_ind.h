#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/ind_data.h"
#include "test_ind_data.h"
#include  <utility>

class TestInd {
public:

    TestInd *makeBlank() {
        return new TestInd();
    }

    void randomize(std::shared_ptr<IndData> &data, Random &rnd) {
        val_ = rnd.nextInt(-1000, 1000);
        //std::cout<<val_<<std::endl;
    }

    std::shared_ptr<TestInd> fromFile(std::string filename) {
        std::shared_ptr<TestInd> newInd = std::make_shared<TestInd>();
        std::cout << "DUMMY file READ" << std::endl;
        newInd->val_ = 43;
        return newInd;
    }

    void toFile(std::string filename) {
        std::cout << "DUMMY file WRITE" << std::endl;
    }

    void mutate(float amount, float probability, Random rnd) {
        val_ += (int) ((rnd.nextFloat(-1000, 1000)) * amount);
    }

    void crossoverTo(TestInd *secondOne, TestInd *ind, Random &rnd) {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);
        TestInd *cSecondOne = dynamic_cast<TestInd *>(secondOne);

        float left = rnd.nextFloat();
        float right = 1 - left;

        cInd->val_ = (int) (left * val_ + right * cSecondOne->val_);
    }

    void mutateTo(float amount, float probability, TestInd *ind, Random &rnd) {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);

        int prev = cInd->val_;
        cInd->val_ = val_ + (int) ((rnd.nextFloat(-1000, 1000)) * amount);


        //if(cInd.val<prev)System.out.println(prev+","+cInd.val);
    }

    void copyTo(TestInd *ind) {
        TestInd *cInd = dynamic_cast<TestInd *>(ind);
        cInd->val_ = val_;
        cInd->fitness = fitness;
        cInd->colX = colX;
        cInd->colY = colY;
    }

    void countFitness(std::shared_ptr<IndData> &data) {
        std::shared_ptr<TestIndData> cData = std::dynamic_pointer_cast<TestIndData>(data);

        fitness = static_cast<int>(val_ * cData->data);
    }

    TestInd *UnsafeClone() {
        TestInd *newInd = new TestInd();
        newInd->val_ = val_;
        newInd->fitness = fitness;
        newInd->colX = colX;
        newInd->colY = colY;

        return newInd;
    }

    std::shared_ptr<TestInd> clone() {
        std::shared_ptr<TestInd> newInd = std::make_shared<TestInd>();
        newInd->val_ = val_;
        newInd->fitness = fitness;
        newInd->colX = colX;
        newInd->colY = colY;

        return newInd;
    }


    std::string toString(std::shared_ptr<IndData> &data) {
        std::shared_ptr<TestIndData> cData = std::dynamic_pointer_cast<TestIndData>(data);
        return std::to_string(val_) + " * " + std::to_string(cData->data);
    }

    void countColor() {
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
