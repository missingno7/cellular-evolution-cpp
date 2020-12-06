#pragma once
//
// Created by jiri on 05/12/20.
//

#include<memory>
#include<cstring>

#include "cellular-evolution/utilities/random.h"

#include "cellular-evolution/cevo/pop_config.h"
#include "img_data.h"
#include "shape.h"
#include "cellular-evolution/utilities/bitmap.hpp"


template<class Individual, class IndData>
class Population;

class ImgIndividual {
public:

    ImgIndividual() {
    }

    ImgIndividual(Random &rnd, ImgData const &data) {
        _r = rnd.nextInt(0,255);
        _g = rnd.nextInt(0,255);
        _b = rnd.nextInt(0,255);
    }

    ~ImgIndividual() {
    }

    ImgIndividual &operator=(const ImgIndividual &individual) {

        _genom=individual._genom;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;

        return *this;
    }

    ImgIndividual(ImgIndividual const &individual) {
        _genom=individual._genom;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;
    }

    void randomize(ImgData const &data, Random &rnd) {
        _genom.clear();
        int n_shapes = rnd.nextInt(1,30);
        for(int i=0;i<n_shapes;i++)
        {
            _genom.push_back(Shape(rnd,data));
        }
    }

    std::shared_ptr<ImgIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void mutate(float amount, float probability, Random &rnd, ImgData const &data) {

    }

    void crossoverTo(ImgIndividual const &second_one, ImgIndividual &ind, Random &rnd) const {

    }

    void mutateTo(float amount, float probability, ImgIndividual &ind, Random &rnd, ImgData const &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(ImgIndividual &ind) {
        // Copies only things that are necessary
        _genom=ind._genom;
    }

    void countFitness(ImgData const &data) {
        fitness = 0;

    }

    void countColor() {
        colX = 0;
        colY = 0;

    }

    std::string toString(ImgData const &data) {
        std::string res = "";

        return res;
    }

    void Draw(ImgData &data, std::string filename) {

        data._gen_bmp.clear(_r, _g, _b);

        data._gen_bmp.Write(filename);
    }

    float getFitness() {
        return fitness;
    }

    std::vector<Shape> _genom;
    float fitness = 0;
    float colX = 0;
    float colY = 0;
    friend class Population<ImgIndividual, ImgData>;
    int _r,_g,_b;

private:
};
