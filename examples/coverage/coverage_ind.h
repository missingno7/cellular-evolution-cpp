#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/utilities/random.h"

#include "cellular-evolution/cevo/pop_config.h"
#include "coverage_data.h"
#include "cellular-evolution/utilities/bitmap.hpp"

template<class Individual, class IndData>
class Population;

class CoverageIndividual {
public:

    CoverageIndividual() {
    }

    CoverageIndividual(CoverageData const &data) {
        _genom_len = data._points.size();
        _genom = new bool[_genom_len];
    }

    ~CoverageIndividual() {
        if (_genom != nullptr) {
            delete[]_genom;
        }
    }

    CoverageIndividual &operator=(const CoverageIndividual &individual) {

        if (_genom_len != individual._genom_len) {
            if (_genom != nullptr) {
                delete[]_genom;
            }

            _genom_len = individual._genom_len;
            _genom = new bool[_genom_len];
        }


        std::memcpy(_genom, individual._genom, _genom_len * sizeof(*_genom));

        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;

        return *this;
    }

    CoverageIndividual(CoverageIndividual const &individual) {
        _genom_len = individual._genom_len;
        _genom = new bool[_genom_len];
        std::memcpy(_genom, individual._genom, _genom_len * sizeof(*_genom));

        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;
    }

    void randomize(CoverageData const &data, Random &rnd) {

        for (int i = 0; i < _genom_len; i++) {
            _genom[i] = rnd.nextBoolean();
        }

        fix_individual(data, rnd);
    }

    std::shared_ptr<CoverageIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void fix_individual(CoverageData const &data, Random &rnd) {
        for (int i = 0; i < data._edges.size(); i++) {

            int first = data._edges[i].first;
            int second = data._edges[i].second;

            if (!_genom[first] && !_genom[second]) {
                if (rnd.nextBoolean()) {
                    _genom[first] = true;
                } else {
                    _genom[second] = true;
                }
            }

        }
    }

    void mutate(float amount, float probability, Random &rnd, CoverageData const &data) {

        for (int i = 0; i < _genom_len; i++) {
            if (rnd.nextFloat() < probability) {
                _genom[i] = !_genom[i];
            }
        }

        fix_individual(data, rnd);
    }

    void crossoverTo(CoverageIndividual const &second_one, CoverageIndividual &ind, Random &rnd,
                     CoverageData const &data) const {

        for (int i = 0; i < _genom_len; i++) {

            if (rnd.nextBoolean()) {
                ind._genom[i] = _genom[i];
            } else {
                ind._genom[i] = second_one._genom[i];
            }
        }
        ind.fix_individual(data, rnd);

    }

    void mutateTo(float amount, float probability, CoverageIndividual &ind, Random &rnd, CoverageData const &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(CoverageIndividual &ind) {
        // Copies only things that are necessary
        std::memcpy(ind._genom, _genom, _genom_len * sizeof(*_genom));
    }

    void countFitness(CoverageData const &data) {
        fitness = 0;

        for (int i = 0; i < _genom_len; i++) {
            if (_genom[i])fitness -= 1.0;
        }
    }

    void countColor(CoverageData const &data) {
        colX = 0;
        colY = 0;

        for (int i = 0; i < _genom_len; i++) {

            if (_genom[i]) {

                switch (i % 4) {
                    case 0:
                        colX += data._points[i].x;
                        colY += data._points[i].y;
                        break;
                    case 1:
                        colX -= data._points[i].x;
                        colY += data._points[i].y;
                        break;
                    case 2:
                        colX += data._points[i].x;
                        colY -= data._points[i].y;
                        break;
                    case 3:
                        colX -= data._points[i].x;
                        colY -= data._points[i].y;
                        break;
                }
            }
        }
    }

    std::string toString(CoverageData const &data) {
        std::string res = "";

        return res;
    }

    void Draw(CoverageData &data, std::string filename) {

        data._bmp.clear(0, 0, 0);

        for (auto const &edge : data._edges) {
            data._bmp.drawLine(data._points[edge.first].x, data._points[edge.first].y, data._points[edge.second].x,
                               data._points[edge.second].y,
                               128,
                               128, 128);
        }


        for (Point const &point: data._points) {
            data._bmp.drawRect(point.x - 2, point.y - 2, point.x + 2, point.y + 2, 255, 0, 0);
        }

        for (int i = 0; i < _genom_len; i++) {

            if (_genom[i]) {
                data._bmp.fillRect(data._points[i].x - 2, data._points[i].y - 2, data._points[i].x + 2,
                                   data._points[i].y + 2, 0, 255, 0);
            }
        }

        data._bmp.Write(filename);
    }

    float getFitness() {
        return fitness;
    }

    bool *_genom = nullptr;
    uint16_t _genom_len = 0;
    float fitness = 0;
    float colX;
    float colY;


    friend class Population<CoverageIndividual, CoverageData>;

private:
};
