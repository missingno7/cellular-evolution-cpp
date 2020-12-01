#pragma once
//
// Created by jiri on 14/11/19.
//

#include<memory>
#include<cstring>

#include "cellular-evolution/cevo/ind_data.h"
//#include "cellular-evolution/cevo/individual.h"
#include "cellular-evolution/utilities/random.h"

#include "cellular-evolution/cevo/pop_config.h"
#include "sa_data.h"
#include "cellular-evolution/utilities/bitmap.hpp"

template<class Individual, class IndData>
class Population;

class SaIndividual {
public:

    SaIndividual() {
    }

    SaIndividual(SaData const &data) {
        genom_len_ = data.cities_;
        scWidth_ = data.scWidth_;
        scHeight_ = data.scHeight_;

        genom_ = new uint16_t[genom_len_];
    }

    ~SaIndividual() {
        if (genom_ != nullptr) {
            delete[]genom_;
        }
    }

    SaIndividual &operator=(const SaIndividual &individual) {

        if (genom_len_ != individual.genom_len_ && genom_ != nullptr) {
            delete[]genom_;
        }

        if (genom_ == nullptr) {
            genom_ = new uint16_t[genom_len_];
            genom_len_ = individual.genom_len_;
        }

        scWidth_ = individual.scWidth_;
        scHeight_ = individual.scHeight_;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;

        std::memcpy(genom_, individual.genom_, genom_len_ * sizeof(*genom_));
    }

    SaIndividual(SaIndividual const &individual) {
        genom_len_ = individual.genom_len_;
        scWidth_ = individual.scWidth_;
        scHeight_ = individual.scHeight_;
        fitness = individual.fitness;
        colX = individual.colX;
        colY = individual.colY;

        genom_ = new uint16_t[genom_len_];
        std::memcpy(genom_, individual.genom_, genom_len_ * sizeof(*genom_));
    }

    void shuffleArray(uint16_t *ar, int size, int from, Random &rnd) {

        for (int i = from; i < size - 1; i++) {
            int index = rnd.nextInt(i + 1, size - 1);
            // Simple swap
            int a = ar[index];
            ar[index] = ar[i];
            ar[i] = a;
        }

    }

    bool checkValidity() {
        bool *test_gen = new bool[genom_len_]();

        for (int i = 0; i < genom_len_; i++) {
            if (test_gen[i]) {
                delete[]test_gen;
                return false;
            }
            test_gen[i] = true;
        }

        delete[]test_gen;
        return true;
    }

    void randomize(SaData const &data, Random &rnd) {
        int maxX = scWidth_;
        int maxY = scHeight_;

        for (int i = 0; i < genom_len_; i++) {
            genom_[i] = i;
        }
        shuffleArray(genom_, genom_len_, 0, rnd);

    }

    std::shared_ptr<SaIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void reversePart(int from, int to) {
        if (from == to) {
            return;
        }

        int frmi = from;
        int toi = to;

        do {

            int tmp = genom_[frmi];
            genom_[frmi] = genom_[toi];
            genom_[toi] = tmp;

            frmi++;
            toi--;

            if (toi < 0) {
                toi = genom_len_ - 1;
            }

            if (frmi >= genom_len_) {
                frmi = 0;
            }

        } while ((frmi - 1) != toi && frmi != toi);
    }

    void shiftMultiple(int from, int to, bool forward, int cnt) {
        if (forward) {
            for (int i = cnt - 1; i >= 0; i--) {

                int frmi = (from + i) % genom_len_;
                int toi = (to + i) % genom_len_;

                //System.out.println(frmi + " + " + toi);
                shift(frmi, toi, true);
            }
        } else {
            // backwards
            for (int i = 0; i < cnt; i++) {

                int frmi = (from + i) % genom_len_;
                int toi = (to + i) % genom_len_;

                shift(frmi, toi, false);
            }
        }
    }

    void shift(int from, int to, bool forward) {

        if (from == to) {
            return;
        }

        if (forward) {

            int i = from;
            do {

                int next = i + 1;
                if (next >= genom_len_) {
                    next = 0;
                }

                int tmp = genom_[i];
                genom_[i] = genom_[next];
                genom_[next] = tmp;

                i = next;
            } while (i != to);

        } else {

            // backwards
            int i = from;
            do {

                int next = (i - 1);
                if (next < 0) {
                    next = genom_len_ - 1;
                }

                int tmp = genom_[i];
                genom_[i] = genom_[next];
                genom_[next] = tmp;

                i = next;
            } while (i != to);
        }
    }

    void mutate(float amount, float probability, Random &rnd, SaData const &data) {

        int swapWith;
        for (int i = 0; i < genom_len_; i++) {

            if (rnd.nextFloat() < probability) {
                swapWith = rnd.nextInt(0, genom_len_ - 1);
                if (swapWith == i) {
                    continue;
                }
                // Simple swap
                int tmp = genom_[i];
                genom_[i] = genom_[swapWith];
                genom_[swapWith] = tmp;
            }
        }

        if (rnd.nextFloat() < data.shiftprob_) {
            // Shifting

            int cnttoshift = rnd.nextInt(0, genom_len_ - 1);
            if (cnttoshift == 0) {
                cnttoshift = 1;
            }

            shiftMultiple(rnd.nextInt(0, genom_len_ - 1), rnd.nextInt(0, genom_len_ - 1), rnd.nextBoolean(),
                          cnttoshift);
        }

        if (rnd.nextFloat() < data.revprob_) {
            reversePart(rnd.nextInt(0, genom_len_ - 1), rnd.nextInt(0, genom_len_ - 1));
        }
    }

    void crossoverTo(SaIndividual &second_one, SaIndividual &ind, Random &rnd) {

        //ind->cities[0] = 0;
        bool *myarray = new bool[genom_len_];
        for (int i = 0; i < genom_len_; i++) {
            myarray[i] = false;
        }

        int crossPoint = rnd.nextInt(0, genom_len_ - 1);
        int i = crossPoint;

        do {
            if ((!myarray[genom_[i]] && !myarray[second_one.genom_[i]])) {
                if (rnd.nextBoolean()) {
                    ind.genom_[i] = genom_[i];
                } else {
                    ind.genom_[i] = second_one.genom_[i];
                }

            } else if (myarray[genom_[i]] && myarray[second_one.genom_[i]]) {

                do {
                    ind.genom_[i] = rnd.nextInt(0, genom_len_ - 1);
                } while (myarray[ind.genom_[i]]);

            } else if (myarray[genom_[i]] && !myarray[second_one.genom_[i]]) {
                ind.genom_[i] = second_one.genom_[i];

            } else if (!myarray[genom_[i]] && myarray[second_one.genom_[i]]) {
                ind.genom_[i] = genom_[i];
            }
            myarray[ind.genom_[i]] = true;

            i = (i + 1) % genom_len_;
        } while ((i % genom_len_) != crossPoint);

        delete[]myarray;

    }

    void mutateTo(float amount, float probability, SaIndividual &ind, Random &rnd, SaData const &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(SaIndividual &ind) {
        // Copies only things that are necessary
        std::memcpy(ind.genom_, genom_, genom_len_ * sizeof(*genom_));
        // No need to copy fitness and color because it will be calculated again
    }

    void DeepCopyTo(SaIndividual &ind) {
        copyTo(ind);
        ind.fitness = fitness;
        ind.colX = colX;
        ind.colY = colY;
    }

    float distance(float x1, float y1, float x2, float y2) {
        float x = (x2 - x1);
        float y = (y2 - y1);
        return (x * x) + (y * y);
    }

    void countFitness(SaData const &data) {
        fitness = 0;

        for (int i = 0; i < genom_len_ - 1; i++) {

            float x1 = data.x[genom_[i]];
            float x2 = data.x[genom_[i + 1]];
            float y1 = data.y[genom_[i]];
            float y2 = data.y[genom_[i + 1]];
            fitness -= distance(x1, y1, x2, y2);

        }

        float x1 = data.x[genom_[0]];
        float x2 = data.x[genom_[genom_len_ - 1]];
        float y1 = data.y[genom_[0]];
        float y2 = data.y[genom_[genom_len_ - 1]];
        fitness -= distance(x1, y1, x2, y2);

    }

    void countColor() {
        colX = 0;
        colY = 0;

        for (int i = 0; i < genom_len_; i++) {
            if (i % 2 == 0) {
                if (i % 4 < 2) {
                    colX += genom_[i];
                } else {
                    colX -= genom_[i];
                }

            } else {
                if (i % 4 < 2) {
                    colY += genom_[i];
                } else {
                    colY -= genom_[i];
                }
            }
        }
    }

    std::string toString(SaData const &data) {
        std::string res = "";

        for (int i = 0; i < genom_len_; i++) {
            res += std::to_string(genom_[i]) + "->";
        }

        return res;
    }

    void Draw(SaData &data, std::string filename) {

        data.bmp_.clear(0, 0, 0);

        for (int i = 0; i < genom_len_ - 1; i++) {
            float x1 = data.x[genom_[i]];
            float x2 = data.x[genom_[i + 1]];
            float y1 = data.y[genom_[i]];
            float y2 = data.y[genom_[i + 1]];

            std::cout << genom_[i] << "-" << genom_[i + 1] << std::endl;

            data.bmp_.drawLine((int) x1, (int) y1, (int) x2, (int) y2, 255, 255, 255);
            data.bmp_.drawRect((int) x1 - 1, (int) y1 - 1, (int) x1 + 3, (int) (y1 + 3), 255, 0, 0);
        }

        float x1 = data.x[genom_[0]];
        float x2 = data.x[genom_[genom_len_ - 1]];
        float y1 = data.y[genom_[0]];
        float y2 = data.y[genom_[genom_len_ - 1]];
        data.bmp_.drawLine((int) x1, (int) y1, (int) x2, (int) y2, 255, 255, 255);
        data.bmp_.drawRect((int) (x2 - 1), (int) (y2 - 1), (int) x2 + 3, (int) (y2 + 3), 255, 0, 0);

        data.bmp_.Write(filename);
    }

    float getFitness() {
        return fitness;
    }

    uint16_t *genom_ = nullptr;
    float fitness;
    float colX;
    float colY;

    static uint16_t scWidth_, scHeight_;
    static uint8_t genom_len_;

    friend class Population<SaIndividual, SaData>;

private:
};

uint8_t SaIndividual::genom_len_ = 0;
uint16_t SaIndividual::scWidth_ = 0;
uint16_t SaIndividual::scHeight_ = 0;

