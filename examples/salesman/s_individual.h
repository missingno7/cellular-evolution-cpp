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

template<typename Individual>
class Population;

class SaIndividual {
public:

    SaIndividual(int scWidth, int scHeight, int cities, std::shared_ptr<PopConfig> &cfg) {

        // Expensive operation, will be called only once
        if (!init_) {
            cfg_ = cfg;
            shiftprob_ = cfg_->reg.getFloat("shiftprob")[0];
            revprob_ = cfg_->reg.getFloat("revprob")[0];
            genom_len_ = cities;
            scWidth_ = scWidth;
            scHeight_ = scHeight;
            bmp_ = std::make_shared<Bitmap>(scWidth_, scHeight_);
            init_ = true;
        }
        genom_ = new uint16_t[genom_len_];

    }

    SaIndividual(int squares) {
        if (!init_) {
            genom_len_ = squares;
        }

        genom_ = new uint16_t[genom_len_];
    }

    SaIndividual() {
        // Only pre-initialised individual can be constructed like that
        assert(genom_len_ != 0);
        genom_ = new uint16_t[genom_len_];
    }

    ~SaIndividual() {
        delete[]genom_;
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


    void randomize(std::shared_ptr<IndData> &data, Random &rnd) {

        std::shared_ptr<SaData> fp_data = std::dynamic_pointer_cast<SaData>(data);

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


    void mutate(float amount, float probability, Random &rnd) {

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

        if (rnd.nextFloat() < shiftprob_) {
            // Shifting

            int cnttoshift = rnd.nextInt(0, genom_len_ - 1);
            if (cnttoshift == 0) {
                cnttoshift = 1;
            }

            shiftMultiple(rnd.nextInt(0, genom_len_ - 1), rnd.nextInt(0, genom_len_ - 1), rnd.nextBoolean(),
                          cnttoshift);
        }

        if (rnd.nextFloat() < revprob_) {
            reversePart(rnd.nextInt(0, genom_len_ - 1), rnd.nextInt(0, genom_len_ - 1));
        }
    }

    void crossoverTo(SaIndividual *second_one, SaIndividual *ind, Random &rnd) {

        //ind->cities[0] = 0;
        bool *myarray = new bool[genom_len_];
        for (int i = 0; i < genom_len_; i++) {
            myarray[i] = false;
        }
        //Arrays.fill(myarray, false);

        // System.out.println("STARTFOR");
        int crossPoint = rnd.nextInt(genom_len_);
        int i = crossPoint;

        do {
            //System.out.println(i);
            //for (int i=0; i<genom_len_ ; i++) {

            if ((!myarray[genom_[i]] && !myarray[second_one->genom_[i]])) {
                if (rnd.nextBoolean()) {
                    ind->genom_[i] = genom_[i];
                } else {
                    ind->genom_[i] = second_one->genom_[i];
                }

            } else if (myarray[genom_[i]] && myarray[second_one->genom_[i]]) {

                //int ic=0;
                do {
                    ind->genom_[i] = rnd.nextInt(genom_len_);
                    //ic++;
                } while (myarray[ind->genom_[i]]);
                //System.out.println(ic);

            } else if (myarray[genom_[i]] && !myarray[second_one->genom_[i]]) {
                ind->genom_[i] = second_one->genom_[i];

            } else if (!myarray[genom_[i]] && myarray[second_one->genom_[i]]) {
                ind->genom_[i] = genom_[i];
            }
            myarray[ind->genom_[i]] = true;

            i = (i + 1) % genom_len_;
        } while ((i % genom_len_) != crossPoint);

        delete[]myarray;

    }

    void mutateTo(float amount, float probability, SaIndividual *ind, Random &rnd) {
        copyTo(ind);
        ind->mutate(amount, probability, rnd);
    }

    void copyTo(SaIndividual *ind) {
        // Copies only things that are necessary
        std::memcpy(ind->genom_, genom_, genom_len_ * sizeof(*genom_));

        // No need to copy fitness and color because it will be calculated again
    }


    void DeepCopyTo(SaIndividual *ind) {
        copyTo(ind);
        ind->fitness = fitness;
        ind->colX = colX;
        ind->colY = colY;
    }


    float distance(float x1, float y1, float x2, float y2) {
        float x = (x2 - x1);
        float y = (y2 - y1);
        return (x * x) + (y * y);

    }


    void countFitness(std::shared_ptr<IndData> &data) {
        std::shared_ptr<SaData> sa_data = std::dynamic_pointer_cast<SaData>(data);

        fitness = 0;

        for (int i = 0; i < genom_len_ - 1; i++) {

            float x1 = sa_data->x[genom_[i]];
            float x2 = sa_data->x[genom_[i + 1]];
            float y1 = sa_data->y[genom_[i]];
            float y2 = sa_data->y[genom_[i + 1]];
            fitness -= distance(x1, y1, x2, y2);

        }

        float x1 = sa_data->x[genom_[0]];
        float x2 = sa_data->x[genom_[genom_len_ - 1]];
        float y1 = sa_data->y[genom_[0]];
        float y2 = sa_data->y[genom_[genom_len_ - 1]];
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

    std::shared_ptr<SaIndividual> clone() {
        std::shared_ptr<SaIndividual> fpInd = std::make_shared<SaIndividual>(scWidth_, scHeight_, genom_len_, cfg_);
        DeepCopyTo(fpInd.get());

        return fpInd;
    }


    std::string toString(std::shared_ptr<IndData> &data) {
        std::string res = "";

        for (int i = 0; i < genom_len_; i++) {
            res += std::to_string(genom_[i]) + "->";
        }

        return res;
    }

    void Draw(std::shared_ptr<IndData> &data, std::string filename) {
        std::shared_ptr<SaData> sa_data = std::dynamic_pointer_cast<SaData>(data);

        bmp_->clear(0, 0, 0);

        for (int i = 0; i < genom_len_ - 1; i++) {
            float x1 = sa_data->x[genom_[i]];
            float x2 = sa_data->x[genom_[i + 1]];
            float y1 = sa_data->y[genom_[i]];
            float y2 = sa_data->y[genom_[i + 1]];

            std::cout << genom_[i] << "-" << genom_[i + 1] << std::endl;

            bmp_->drawLine((int) x1, (int) y1, (int) x2, (int) y2, 255, 255, 255);
            bmp_->drawRect((int) x1 - 1, (int) y1 - 1, (int) x1 + 3, (int) (y1 + 3), 255, 0, 0);

        }

        float x1 = sa_data->x[genom_[0]];
        float x2 = sa_data->x[genom_[genom_len_ - 1]];
        float y1 = sa_data->y[genom_[0]];
        float y2 = sa_data->y[genom_[genom_len_ - 1]];
        bmp_->drawLine((int) x1, (int) y1, (int) x2, (int) y2, 255, 255, 255);
        bmp_->drawRect((int) (x1 - 1), (int) (y1 - 1), (int) x1 + 3, (int) (y1 + 3), 255, 0, 0);

        bmp_->Write(filename);
    }

    float getFitness() {
        return fitness;
    }

    uint16_t *genom_;
    float fitness;
    float colX;
    float colY;

    static uint16_t scWidth_, scHeight_;
    static uint8_t genom_len_;
    static bool init_;
    static float shiftprob_;
    static float revprob_;
    static float mut_prob_;
    static float mut_amount_;
    static std::shared_ptr<PopConfig> cfg_;
    static std::shared_ptr<Bitmap> bmp_;

    friend class Population<SaIndividual>;

private:

    // Unsafe but fast methods that can lead to memleaks when misused.
    SaIndividual *UnsafeClone() {
        SaIndividual *fpInd = new SaIndividual(scWidth_, scHeight_, genom_len_, cfg_);
        DeepCopyTo(fpInd);

        return fpInd;
    }

    SaIndividual *makeBlank() {
        return new SaIndividual(scWidth_, scHeight_, genom_len_, cfg_);
    }


};

std::shared_ptr<Bitmap> SaIndividual::bmp_ = 0;

float SaIndividual::shiftprob_;
float SaIndividual::revprob_;
uint8_t SaIndividual::genom_len_ = 0;
uint16_t SaIndividual::scWidth_;
uint16_t SaIndividual::scHeight_;

bool SaIndividual::init_ = false;
std::shared_ptr<PopConfig> SaIndividual::cfg_ = 0;
