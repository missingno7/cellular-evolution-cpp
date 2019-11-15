#pragma once
//
// Created by jiri on 14/11/19.
//

#include <string>
#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/utilities/random.h"

#include <memory>

class Individual {
public:

virtual Individual *makeBlank()=0;

    virtual void randomize(std::shared_ptr<IndData> &data,Random &rnd)=0;

    virtual std::shared_ptr<Individual> fromFile(std::string filename)=0;

    virtual void toFile(std::string filename)=0;

    virtual void mutate(float amount, float probability, Random rnd)=0;

    virtual void crossoverTo(Individual *secondOne, Individual *ind, Random &rnd)=0;

    virtual void mutateTo(float amount, float probability, Individual *ind, Random &rnd)=0;

    virtual void copyTo(Individual *ind)=0;

    virtual void countFitness(std::shared_ptr<IndData> &data)=0;

    virtual void countColor()=0;

    virtual Individual *clone()=0;

    virtual std::string toString(std::shared_ptr<IndData> &data)=0;

    friend class Population;

float getFitness() {
        return fitness;
    }

static bool isPrintable() {
        return false;
    }

static bool isDrawable() {
        return false;
    }

protected:

float fitness;
float colX,colY;

};