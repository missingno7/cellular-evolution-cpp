#pragma once
//
// Created by jiri on 14/11/19.
//

#include<memory>
#include<cstring>

#include "cellular-evolution/utilities/random.h"

#include "cellular-evolution/cevo/pop_config.h"
#include "rectangle.h"
#include "fp_data.h"
#include "cellular-evolution/utilities/bitmap.hpp"

template<class Individual, class IndData>
class Population;

class FpIndividual {
public:

    FpIndividual(FpData const &data) {
        genom_ = new Rectangle[data.squares];
        genom_len_ = data.squares;
    }

    FpIndividual() {
    }

    FpIndividual(FpIndividual const &individual) {
        genom_ = new Rectangle[individual.genom_len_];
        genom_len_ = individual.genom_len_;
        std::memcpy(genom_, individual.genom_, genom_len_ * sizeof(*genom_));
        fitness = individual.fitness;
        colX=individual.colX;
        colY=individual.colY;
    }


    FpIndividual &operator=(const FpIndividual &individual) {
        if (genom_len_ != individual.genom_len_)
        {
            if (genom_ != nullptr )
            {
                delete []genom_;
            }
        }

        if (genom_==nullptr)
        {
            genom_ = new Rectangle[individual.genom_len_];
            genom_len_ = individual.genom_len_;
        }

        std::memcpy(genom_, individual.genom_, genom_len_ * sizeof(*genom_));
        fitness = individual.fitness;
        colX=individual.colX;
        colY=individual.colY;
    }



    ~FpIndividual() {
        if (genom_ != nullptr)
        {
        delete[]genom_;
        }
    }

    bool overlap2D(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
        return !(bx2 <= ax1 || ax2 <= bx1 || by2 <= ay1 || ay2 <= by1);
    }

    void randomize(FpData const &data, Random &rnd) {

        int maxX = data.scWidth;
        int maxY = data.scHeight;

        int attempts = 0;

        for (int i = 0; i < genom_len_; i++) {
            bool again;
            bool flip;
            do {
                flip = rnd.nextBoolean();

                genom_[i].x1 = rnd.nextInt(100, maxX - 100);
                genom_[i].y1 = rnd.nextInt(100, maxY - 100);

                if (flip) {

                    genom_[i].x2 = genom_[i].x1 + data.height[i];
                    genom_[i].y2 = genom_[i].y1 + data.width[i];

                } else {
                    genom_[i].x2 = genom_[i].x1 + data.width[i];
                    genom_[i].y2 = genom_[i].y1 + data.height[i];
                }

                again = false;
                // Colisions

                if (attempts < 10000) {
                    for (int j = 0; j < i; j++) {
                        if (overlap2D(genom_[i].x1, genom_[i].y1, genom_[i].x2, genom_[i].y2, genom_[j].x1,
                                      genom_[j].y1, genom_[j].x2, genom_[j].y2)) {
                            again = true;
                            attempts++;
                        }
                    }
                } else {
                    std::cout << "Space is too small" << std::endl;
                }

            } while (again);
        }
    }

    std::shared_ptr<FpIndividual> fromFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) {
        throw std::runtime_error("Not supported yet.");
    }

    void mutate(float amount, float probability, Random &rnd, FpData &data) {
        for (int i = 0; i < genom_len_; i++) {
            // Move
            if (rnd.nextFloat() < probability) {
                float mutX = rnd.nextNormalFloat(0.0, 1.0) * amount;
                if (genom_[i].x2 + mutX >= data.scWidth || genom_[i].x1 + mutX < 0) {
                    genom_[i].x1 -= mutX;
                    genom_[i].x2 -= mutX;
                } else {
                    genom_[i].x1 += mutX;
                    genom_[i].x2 += mutX;
                }
                float mutY = (rnd.nextNormalFloat(0.0, 1.0) * amount);
                if (genom_[i].y2 + mutY >= data.scHeight || genom_[i].y1 + mutY < 0) {
                    genom_[i].y1 -= mutY;
                    genom_[i].y2 -= mutY;
                } else {
                    genom_[i].y1 += mutY;
                    genom_[i].y2 += mutY;
                }
            }
            // Flip
            if (rnd.nextFloat() < data.flip_prob_) {
                genom_[i].flip();
            }
            //Switch
            if (rnd.nextFloat() < data.switch_prob_) {
                int sw = rnd.nextInt(0, genom_len_ - 1);

                int tmpx = genom_[i].x1 - genom_[sw].x1;
                genom_[i].x1 = genom_[i].x1 - tmpx;
                genom_[i].x2 = genom_[i].x2 - tmpx;
                genom_[sw].x1 = genom_[sw].x1 + tmpx;
                genom_[sw].x2 = genom_[sw].x2 + tmpx;


                int tmpy = genom_[i].y1 - genom_[sw].y1;
                genom_[i].y1 = genom_[i].y1 - tmpy;
                genom_[i].y2 = genom_[i].y2 - tmpy;
                genom_[sw].y1 = genom_[sw].y1 + tmpy;
                genom_[sw].y2 = genom_[sw].y2 + tmpy;
            }
        }
    }

    void crossoverTo(FpIndividual const &second_one, FpIndividual &ind, Random &rnd) const{

        for (int i = 0; i < genom_len_; i++) {

            int resx, resy;
            if ((genom_[i].x1) > (second_one.genom_[i].x1)) {
                resx = rnd.nextInt(second_one.genom_[i].x1, genom_[i].x1);
            } else {
                resx = rnd.nextInt(genom_[i].x1, second_one.genom_[i].x1);
            }


            if ((genom_[i].y1) > (second_one.genom_[i].y1)) {
                resy = rnd.nextInt(second_one.genom_[i].y1, genom_[i].y1);
            } else {
                resy = rnd.nextInt(genom_[i].y1, second_one.genom_[i].y1);
            }


            if (rnd.nextBoolean()) {
                ind.genom_[i].x2 = resx + (genom_[i].x2 - genom_[i].x1);
                ind.genom_[i].x1 = resx;

                ind.genom_[i].y2 = resy + (genom_[i].y2 - genom_[i].y1);
                ind.genom_[i].y1 = resy;
            } else {

                ind.genom_[i].x2 = resx + (second_one.genom_[i].x2 - second_one.genom_[i].x1);
                ind.genom_[i].x1 = resx;

                ind.genom_[i].y2 = resy + (second_one.genom_[i].y2 - second_one.genom_[i].y1);
                ind.genom_[i].y1 = resy;
            }
        }
    }

    void mutateTo(float amount, float probability, FpIndividual &ind, Random &rnd, FpData &data) {
        copyTo(ind);
        ind.mutate(amount, probability, rnd, data);
    }

    void copyTo(FpIndividual &ind) {

        // Copies only things that are necessary
        std::memcpy(ind.genom_, genom_, genom_len_ * sizeof *genom_);

        // No need to copy fitness and color because it will be calculated again
    }

    void moveToCenter(FpData const &data) {

        int sumX = 0, sumY = 0;

        for (int i = 0; i < genom_len_; i++) {
            sumX += genom_[i].x1;
            sumY += genom_[i].y1;
        }

        sumX = sumX / genom_len_;
        sumY = sumY / genom_len_;

        int centerX = data.scWidth / 2;
        int centerY = data.scHeight / 2;

        int moveX = sumX - centerX;
        int moveY = sumY - centerY;

        for (int i = 0; i < genom_len_; i++) {
            genom_[i].x1 -= moveX;
            genom_[i].y1 -= moveY;
            genom_[i].x2 -= moveX;
            genom_[i].y2 -= moveY;
        }

    }

    void countFitness(FpData const &data) {
        moveToCenter(data);
        fitness = 0;

        // Colisions
        for (int i = 0; i < genom_len_; i++) {
            for (int j = i + 1; j < genom_len_; j++) {
                if (overlap2D(genom_[i].x1, genom_[i].y1, genom_[i].x2, genom_[i].y2, genom_[j].x1, genom_[j].y1,
                              genom_[j].x2, genom_[j].y2)) {
                    float penalty = data.width[i] * data.height[i] + data.width[j] * data.height[j];
                    fitness -= penalty * 3;
                }
            }
        }

        // bounding box surface
        float maxX = genom_[0].x2;
        float maxY = genom_[0].y2;
        float minX = genom_[0].x1;
        float minY = genom_[0].y1;

        for (int i = 1; i < genom_len_; i++) {
            if (maxX < genom_[i].x2) {
                maxX = genom_[i].x2;
            }

            if (minX > genom_[i].x1) {
                minX = genom_[i].x1;
            }

            if (maxY < genom_[i].y2) {
                maxY = genom_[i].y2;
            }

            if (minY > genom_[i].y1) {
                minY = genom_[i].y1;
            }

        }
        fitness -= (maxX - minX) * (maxY - minY);
    }

    void countColor() {
        colX = 0;
        colY = 0;

        for (int i = 0; i < genom_len_; i++) {
            if (i % 2 == 0) {
                colX += genom_[i].x1;
                colY += genom_[i].y1;
            } else {
                colX -= genom_[i].x1;
                colY -= genom_[i].y1;
            }
        }
    }

    std::string toString(FpData const &data) {
        std::string res = "";

        for (int i = 0; i < genom_len_; i++) {
            res += std::to_string((int) genom_[i].x1) + "," + std::to_string((int) genom_[i].y1) + "\n";
        }

        return res;
    }

    void Draw(FpData &data, std::string filename) {
        // bounding box surface
        uint16_t maxX = genom_[0].x2;
        uint16_t maxY = genom_[0].y2;
        uint16_t minX = genom_[0].x1;
        uint16_t minY = genom_[0].y2;

        for (int i = 1; i < genom_len_; i++) {
            if (maxX < genom_[i].x2) {
                maxX = genom_[i].x2;
            }

            if (minX > genom_[i].x1) {
                minX = genom_[i].x1;
            }

            if (maxY < genom_[i].y2) {
                maxY = genom_[i].y2;
            }

            if (minY > genom_[i].y1) {
                minY = genom_[i].y1;
            }
        }


        data.bmp_.clear(0, 0, 0);

        // Bounding box
        data.bmp_.fillRect((int) minX, (int) minY, (int) (
                maxX), (int) (maxY), 96, 0, 0);

        for (int i = 0; i < genom_len_; i++) {

            data.bmp_.fillRect((int) genom_[i].x1, (int) genom_[i].y1, (int) genom_[i].x2, (int) genom_[i].y2, 255, 255,
                           255);

            data.bmp_.drawRect((int) genom_[i].x1, (int) genom_[i].y1, (int) genom_[i].x2, (int) genom_[i].y2, 255, 0,
                           0);
        }

        data.bmp_.Write(filename);
    }

    float getFitness() {
        return fitness;
    }

    Rectangle *genom_ = nullptr;
    uint16_t genom_len_;
    float fitness;
    float colX;
    float colY;

    friend class Population<FpIndividual, FpData>;
};

