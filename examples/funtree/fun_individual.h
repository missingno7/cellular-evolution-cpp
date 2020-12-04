#pragma once
//
// Created by jiri on 14/11/19.
//

#include<memory>
#include<cstring>
#include<limits>

#include "cellular-evolution/utilities/random.h"

#include "cellular-evolution/cevo/pop_config.h"
#include "fun_data.h"
#include "expression.cpp"
#include "cellular-evolution/utilities/bitmap.hpp"

template<class Individual, class IndData>
class Population;

class FunIndividual {
public:

    FunIndividual(FunData const &data) {

    }

    FunIndividual() {
    }

    FunIndividual(FunIndividual const &individual) {
        if (individual._exp != nullptr) {
            _exp = individual._exp->clone();
        }

        fitness = individual.fitness;
        colY = individual.colY;
        colX = individual.colX;
    }


    FunIndividual &operator=(const FunIndividual &individual) {
        if (_exp != nullptr) {
            delete _exp;
        }

        if (individual._exp != nullptr) {
            _exp = individual._exp->clone();
        }

        fitness = individual.fitness;
        colY = individual.colY;
        colX = individual.colX;
    }


    ~FunIndividual() {
        if (_exp != nullptr) {
            delete _exp;
        }
    }


    void randomize(FunData const &data, Random &rnd) {
        if (_exp != nullptr) {
            delete _exp;
        }

        int min = rnd.nextInt(0,10);
        int max = rnd.nextInt(0,10);
        if (min>max)
        {
            int tmp = max;
            max = min;
            min = tmp;
        }


        _exp = Expression::makeRandom(rnd, min, max);
    }

    std::shared_ptr<FunIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void mutate(float amount, float probability, Random &rnd, FunData &data) {
        Expression::mutate(_exp, rnd);
    }

    void crossoverTo(FunIndividual const &second_one, FunIndividual &ind, Random &rnd) const {
        throw std::runtime_error("Not supported yet.");
    }

    void mutateTo(float amount, float probability, FunIndividual &ind, Random &rnd, FunData &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(FunIndividual &ind) {

        if (ind._exp != nullptr) {
            delete ind._exp;
        }
        ind._exp = _exp->clone();
    }

    void countFitness(FunData const &data) {
        fitness = 0;

        for (auto point : data.points) {
            float y = _exp->evaluate(point.first);
            if (!is_valid_value(y)) {
                y = 0;
            }

            fitness -= powf(point.second - y, 2);
        }


        if ((!is_valid_value(fitness))) {
            fitness = std::numeric_limits<float>::lowest();
            return;
        }
    }

    void countColor() {

        colX = 0;
        colY = 0;

        if (_exp != nullptr) {
            _exp->countColor(colX, colY);
        }
    }

    std::string toString(FunData const &data) {
        if (_exp == nullptr) {
            return "NULL";
        } else {
            std::string res = _exp->toString() + "\n";

            res += "X:\tY_PRED:\tY_TRUE\n";

            for (auto &point : data.points) {
                res += std::to_string(point.first) +=
                        "\t" + std::to_string(_exp->evaluate(point.first)) + "\t" + std::to_string(point.second) + "\n";
            }

            return res;
        }
    }

    void Draw(FunData &data, std::string filename) {
        data.bmp_.clear(0, 0, 0);

        std::vector<float> x;
        std::vector<float> y;
        std::vector<float> y_pred;

        float x_min = data.points[0].first;
        float x_max = data.points[0].first;
        float y_min = data.points[0].second;
        float y_max = data.points[0].second;


        for (auto &point : data.points) {
            float current_x = point.first;
            float current_y = -point.second;
            float current_y_pred = -_exp->evaluate(point.first);


            if (!is_valid_value(current_y_pred)) {
                current_y_pred = 0;
            }

            if (current_x < x_min) {
                x_min = current_x;
            }
            if (current_x > x_max) {
                x_max = current_x;
            }


            if (current_y < y_min) {
                y_min = current_y;
            }
            if (current_y > y_max) {
                y_max = current_y;
            }


            if (current_y_pred < y_min) {
                y_min = current_y_pred;
            }
            if (current_y_pred > y_max) {
                y_max = current_y_pred;
            }

            x.push_back(current_x);
            y.push_back(current_y);
            y_pred.push_back(current_y_pred);


        }

        float x_diff = x_max - x_min;
        float y_diff = y_max - y_min;


        // Draw ground truth
        for (int i = 0; i < x.size() - 1; i++) {
            int x1 = int(10.0 + ((x[i] - x_min) / x_diff) * (data.scWidth - 20.0));
            int y1 = int(10.0 + ((y[i] - y_min) / y_diff) * (data.scHeight - 20.0));
            int x2 = int(10.0 + ((x[i + 1] - x_min) / x_diff) * (data.scWidth - 20.0));
            int y2 = int(10.0 + ((y[i + 1] - y_min) / y_diff) * (data.scHeight - 20.0));
            data.bmp_.drawLine(x1, y1, x2, y2, 255, 255, 0);
        }


        // Draw prediction
        for (int i = 0; i < x.size() - 1; i++) {
            int x1 = int(10.0 + ((x[i] - x_min) / x_diff) * (data.scWidth - 20.0));
            int y1 = int(10.0 + ((y_pred[i] - y_min) / y_diff) * (data.scHeight - 20.0));
            int x2 = int(10.0 + ((x[i + 1] - x_min) / x_diff) * (data.scWidth - 20.0));
            int y2 = int(10.0 + ((y_pred[i + 1] - y_min) / y_diff) * (data.scHeight - 20.0));
            data.bmp_.drawLine(x1, y1, x2, y2, 0, 255, 255);
        }


        data.bmp_.Write(filename);
    }

    inline float getFitness() {
        return -logf(-fitness);
    }


protected:
    float fitness = 0;
    float colX = 0;
    float colY = 0;

private:
    Expression *_exp = nullptr;

    bool is_valid_value(float num) {
        return (!std::isnan(num) & std::isfinite(num));
    }

    friend class Population<FunIndividual, FunData>;
};

