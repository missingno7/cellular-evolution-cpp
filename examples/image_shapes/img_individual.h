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
        _r = rnd.nextInt(0, 255);
        _g = rnd.nextInt(0, 255);
        _b = rnd.nextInt(0, 255);
    }

    ~ImgIndividual() {
    }

    ImgIndividual &operator=(const ImgIndividual &individual) {

        _genom = individual._genom;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;
        _r = individual._r;
        _g = individual._g;
        _b = individual._b;

        return *this;
    }

    ImgIndividual(ImgIndividual const &individual) {
        _genom = individual._genom;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;
        _r = individual._r;
        _g = individual._g;
        _b = individual._b;
    }

    void randomize(ImgData const &data, Random &rnd) {
        _genom.clear();
        //int n_shapes = rnd.nextInt(1, 30);

        _r = rnd.nextInt(0, 255);
        _g = rnd.nextInt(0, 255);
        _b = rnd.nextInt(0, 255);

        int n_shapes = 20;
        for (int i = 0; i < n_shapes; i++) {
            _genom.push_back(Shape(rnd, data));
        }
    }

    std::shared_ptr<ImgIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void mutate(float amount, float probability, Random &rnd, ImgData const &data) {
        for (Shape &shp : _genom) {
            if (rnd.nextFloat() < probability) {
                shp.mutate(rnd, data);
            }
        }


        for (int i = 0; i < _genom.size(); i++) {
            if (rnd.nextFloat() < probability) {
                int r_i = rnd.nextInt(0, _genom.size() - 1);
                if (i == r_i)continue;
                std::swap(_genom[i], _genom[r_i]);
            }
        }

        if (rnd.nextFloat() < probability) {

            // Mutate color
            int r_mut = rnd.nextNormalFloat(0, 10);
            if (_r + r_mut > 255 || _r + r_mut < 0) {
                _r -= r_mut;
            } else {
                _r += r_mut;
            }
            int g_mut = rnd.nextNormalFloat(0, 10);
            if (_g + g_mut > 255 || _g + g_mut < 0) {
                _g -= g_mut;
            } else {
                _g += g_mut;
            }
            int b_mut = rnd.nextNormalFloat(0, 10);
            if (_b + b_mut > 255 || _b + b_mut < 0) {
                _b -= b_mut;
            } else {
                _b += b_mut;
            }

        }

    }

    void crossoverTo(ImgIndividual const &second_one, ImgIndividual &ind, Random &rnd) const {
        float ratio = rnd.nextFloat();
        ind._r = (1 - ratio) * _r + ratio * second_one._r;
        ind._g = (1 - ratio) * _g + ratio * second_one._g;
        ind._b = (1 - ratio) * _b + ratio * second_one._b;

        ind._genom.clear();
        ind._genom.reserve(_genom.size());

        // Unsafe for different sizes
        for (int i = 0; i < _genom.size(); i++) {
            if (rnd.nextBoolean()) {
                ind._genom[i] = _genom[i];
            } else {
                ind._genom[i] = second_one._genom[i];
            }
        }
    }

    void mutateTo(float amount, float probability, ImgIndividual &ind, Random &rnd, ImgData const &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(ImgIndividual &ind) {
        // Copies only things that are necessary
        ind._genom = _genom;
        ind._r = _r;
        ind._g = _g;
        ind._b = _b;
    }

    void countFitness(ImgData &data) {
        fitness = 0;
        prepareImg(data);


        for (int i = 0; i < data._sc_width * data._sc_height * 3; i++) {
            fitness -= abs(data._gen_bmp.getRaw(i) - data._gt_bmp.getRaw(i));
        }

    }

    void countColor() {
        colX = 0;
        colY = 0;

        for (Shape const &shape : _genom) {
            shape.countColor(colX, colY);
        }

        colX += _r - 128;
        colX += _g - 128;
        colX += _b - 128;
    }

    std::string toString(ImgData const &data) {
        std::string res = "";

        return res;
    }

    void Draw(ImgData &data, std::string filename) {

        data._gen_bmp.clear(_r, _g, _b);
        prepareImg(data);

        data._gen_bmp.Write(filename);
    }


    void prepareImg(ImgData &data) {
        data._gen_bmp.clear(_r, _g, _b);
        for (Shape const &shape : _genom) {
            shape.draw(data._gen_bmp);
        }

    }


    float getFitness() {
        return fitness;
    }

    std::vector<Shape> _genom;
    float fitness = 0;
    float colX = 0;
    float colY = 0;

    friend class Population<ImgIndividual, ImgData>;

    int _r, _g, _b;

private:
};
