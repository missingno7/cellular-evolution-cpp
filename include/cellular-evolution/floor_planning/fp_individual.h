#pragma once
//
// Created by jiri on 14/11/19.
//

#include<memory>
#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/cevo/individual.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/floor_planning/rectangle.h"
#include "cellular-evolution/floor_planning/fp_data.h"
#include "cellular-evolution/utilities/bitmap.hpp"

class FpIndividual : public Individual {
public:

    FpIndividual(int scWidth, int scHeight, int squares, std::shared_ptr<PopConfig> &cfg) {

        scWidth_ = scWidth;
        scHeight_ = scHeight;

        if (!bmp_) {
            bmp_ = std::make_shared<Bitmap>(scWidth_, scHeight_);
        }

        cfg_ = cfg;
        genom_ = new Rectangle *[squares];
        genom_len_ = squares;

        for (int i = 0; i < squares; i++) {
            genom_[i] = new Rectangle();
        }

        // Expensive operation, will be called only once
        if (!init_) {
            flip_prob_ = cfg_->reg.getFloat("flipprob")[0];
            switch_prob_ = cfg_->reg.getFloat("switchprob")[0];
            init_ = true;
        }
    }

    Individual *makeBlank() override {
        return new FpIndividual(scWidth_, scHeight_, genom_len_, cfg_);
    }

    bool overlap2D(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
        return !(bx2 <= ax1 || ax2 <= bx1 || by2 <= ay1 || ay2 <= by1);
    }

    void randomize(std::shared_ptr<IndData> &data, Random &rnd) override {

        int maxX = scWidth_;
        int maxY = scHeight_;

        std::shared_ptr<FpData> fp_data = std::dynamic_pointer_cast<FpData>(data);

        int attempts = 0;

        /*flip_prob_ = rnd.nextFloat();
        switch_prob_ = rnd.nextFloat();
        mut_prob_ = rnd.nextFloat();
        mut_amount_ = (float) rnd.nextGaussian();*/
        for (int i = 0; i < genom_len_; i++) {

            bool again;
            bool flip;
            do {
                flip = rnd.nextBoolean();

                genom_[i]->x1 = rnd.nextInt(100, maxX - 100);
                genom_[i]->y1 = rnd.nextInt(100, maxY - 100);
                //System.out.println(x[i]+"AND"+y[i]);

                if (flip) {

                    genom_[i]->x2 = genom_[i]->x1 + fp_data->height[i];
                    genom_[i]->y2 = genom_[i]->y1 + fp_data->width[i];

                } else {
                    genom_[i]->x2 = genom_[i]->x1 + fp_data->width[i];
                    genom_[i]->y2 = genom_[i]->y1 + fp_data->height[i];
                }

                again = false;
                // Colisions

                if (attempts < 10000) {
                    for (int j = 0; j < i; j++) {
                        if (overlap2D(genom_[i]->x1, genom_[i]->y1, genom_[i]->x2, genom_[i]->y2, genom_[j]->x1,
                                      genom_[j]->y1, genom_[j]->x2, genom_[j]->y2)) {
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

    std::shared_ptr<Individual> fromFile(std::string filename) override {
        throw std::runtime_error("Not supported yet.");
    }

    void toFile(std::string filename) override {
        throw std::runtime_error("Not supported yet.");
    }

    void mutate(float amount, float probability, Random rnd) override {

        /*flip_prob_ += (float) rnd.nextGaussian() * probability;
        switch_prob_ += (float) rnd.nextGaussian() * probability;
        mut_prob_ += (float) rnd.nextGaussian() * probability;
        mut_amount_ += (float) rnd.nextGaussian() * amount;*/

        for (int i = 0; i < genom_len_; i++) {
            // Move
            if (rnd.nextFloat() < probability) {
                float mutX = (float) rnd.nextFloat(-2.0, 2.0) * amount;
                if (genom_[i]->x1 + mutX >= scWidth_ || genom_[i]->x1 + mutX < 0) {
                    genom_[i]->x1 -= mutX;
                    genom_[i]->x2 -= mutX;
                } else {
                    genom_[i]->x1 += mutX;
                    genom_[i]->x2 += mutX;
                }
                float mutY = (float) (rnd.nextFloat(-2.0, 2.0) * amount);
                if (genom_[i]->y1 + mutY >= scHeight_ || genom_[i]->y1 + mutY < 0) {
                    genom_[i]->y1 -= mutY;
                    genom_[i]->y2 -= mutY;
                } else {
                    genom_[i]->y1 += mutY;
                    genom_[i]->y2 += mutY;
                }
            }
            // Flip
            if (rnd.nextFloat() < flip_prob_) {
                genom_[i]->flip();
            }
            //Switch
            if (rnd.nextFloat() < switch_prob_) {
                int sw = rnd.nextInt(0, genom_len_ - 1);
                int tmpx = genom_[i]->x1 - genom_[sw]->x1;
                genom_[i]->x1 = genom_[i]->x1 - tmpx;
                genom_[i]->x2 = genom_[i]->x2 - tmpx;
                int tmpy = genom_[i]->y1 - genom_[sw]->y1;
                genom_[i]->y1 = genom_[i]->y1 - tmpy;
                genom_[i]->y2 = genom_[i]->y2 - tmpy;
            }
        }
    }

    void crossoverTo(Individual *secondOne, Individual *ind, Random &rnd) override {
        //FpIndividual cInd = (FpIndividual) ind;
        //FpIndividual cSecondOne = (FpIndividual) secondOne;

        FpIndividual *cInd = dynamic_cast<FpIndividual *>(ind);
        FpIndividual *cSecondOne = dynamic_cast<FpIndividual *>(secondOne);

        /*float ratio = rnd.nextFloat();
        cInd.flip_prob_ = ratio * flip_prob_ + (1 - ratio) * cSecondOne.flip_prob_;
        ratio = rnd.nextFloat();
        cInd.switch_prob_ = ratio * switch_prob_ + (1 - ratio) * cSecondOne.switch_prob_;
        ratio = rnd.nextFloat();
        cInd.mut_prob_ = ratio * mut_prob_ + (1 - ratio) * cSecondOne.mut_prob_;
        ratio = rnd.nextFloat();
        cInd.mut_amount_ = ratio * mut_amount_ + (1 - ratio) * cSecondOne.mut_amount_;*/
        for (int i = 0; i < genom_len_; i++) {
            if (rnd.nextBoolean()) {
                cInd->genom_[i]->x1 = genom_[i]->x1;
                cInd->genom_[i]->x2 = genom_[i]->x2;
                cInd->genom_[i]->y1 = genom_[i]->y1;
                cInd->genom_[i]->y2 = genom_[i]->y2;
            } else {
                cInd->genom_[i]->x1 = cSecondOne->genom_[i]->x1;
                cInd->genom_[i]->x2 = cSecondOne->genom_[i]->x2;
                cInd->genom_[i]->y1 = cSecondOne->genom_[i]->y1;
                cInd->genom_[i]->y2 = cSecondOne->genom_[i]->y2;
            }
        }
    }

    void mutateTo(float amount, float probability, Individual *ind, Random &rnd) override {
        copyTo(ind);
        ind->mutate(amount, probability, rnd);
    }

    void copyTo(Individual *ind) override {
        FpIndividual *fpInd = dynamic_cast<FpIndividual *>(ind);

        for (int i = 0; i < genom_len_; i++) {
            genom_[i]->copyTo(fpInd->genom_[i]);
        }

        fpInd->fitness = fitness;
        fpInd->colX = colX;
        fpInd->colY = colY;
        /*fpInd.flip_prob_=flip_prob_;
        fpInd.switch_prob_=switch_prob_;
        fpInd.mut_prob_=mut_prob_;
        fpInd.mut_amount_=mut_amount_;*/
    }

    void moveToCenter() {

        int sumX = 0, sumY = 0;

        for (int i = 0; i < genom_len_; i++) {
            sumX += genom_[i]->x1;
            sumY += genom_[i]->y1;
        }

        sumX = sumX / genom_len_;
        sumY = sumY / genom_len_;

        int centerX = scWidth_ / 2;
        int centerY = scHeight_ / 2;

        int moveX = sumX - centerX;
        int moveY = sumY - centerY;

        for (int i = 0; i < genom_len_; i++) {
            genom_[i]->x1 -= moveX;
            genom_[i]->y1 -= moveY;
            genom_[i]->x2 -= moveX;
            genom_[i]->y2 -= moveY;
        }

    }

    void countFitness(std::shared_ptr<IndData> &data) override {
        std::shared_ptr<FpData> fp_data = std::dynamic_pointer_cast<FpData>(data);

        moveToCenter();
        fitness = 0;

        // Colisions
        for (int i = 0; i < genom_len_; i++) {
            for (int j = i + 1; j < genom_len_; j++) {
                if (overlap2D(genom_[i]->x1, genom_[i]->y1, genom_[i]->x2, genom_[i]->y2, genom_[j]->x1, genom_[j]->y1,
                              genom_[j]->x2, genom_[j]->y2)) {
                    float penalty = fp_data->width[i] * fp_data->height[i] + fp_data->width[j] * fp_data->height[j];
                    fitness -= penalty * 3;
                }
            }
        }

        // bounding box surface
        float maxX = genom_[0]->x2;
        float maxY = genom_[0]->y2;
        float minX = genom_[0]->x1;
        float minY = genom_[0]->y1;

        for (int i = 1; i < genom_len_; i++) {
            if (maxX < genom_[i]->x2) {
                maxX = genom_[i]->x2;
            }

            if (minX > genom_[i]->x1) {
                minX = genom_[i]->x1;
            }

            if (maxY < genom_[i]->y2) {
                maxY = genom_[i]->y2;
            }

            if (minY > genom_[i]->y1) {
                minY = genom_[i]->y1;
            }

        }
        fitness -= (maxX - minX) * (maxY - minY);
    }

    void countColor() override {
        colX = 0;
        colY = 0;

        for (int i = 0; i < genom_len_; i++) {
            if (i % 2 == 0) {
                colX += genom_[i]->x1;
                colY += genom_[i]->y1;
            } else {
                colX -= genom_[i]->x1;
                colY -= genom_[i]->y1;
            }
        }
    }

    Individual *clone() override {
        FpIndividual *fpInd = new FpIndividual(scWidth_, scHeight_, genom_len_, cfg_);
        copyTo(fpInd);

        return fpInd;
    }

    std::string toString(std::shared_ptr<IndData> &data) override {
        std::string res = "";

        for (int i = 0; i < genom_len_; i++) {
            res += std::to_string((int) genom_[i]->x1) + "," + std::to_string((int) genom_[i]->y1) + "\n";
        }

        return res;
    }

    void Draw(std::shared_ptr<IndData> &data, std::string filename) {
        // bounding box surface
        float maxX = genom_[0]->x2;
        float maxY = genom_[0]->y2;
        float minX = genom_[0]->x1;
        float minY = genom_[0]->y2;

        for (int i = 1; i < genom_len_; i++) {
            if (maxX < genom_[i]->x2) {
                maxX = genom_[i]->x2;
            }

            if (minX > genom_[i]->x1) {
                minX = genom_[i]->x1;
            }

            if (maxY < genom_[i]->y2) {
                maxY = genom_[i]->y2;
            }

            if (minY > genom_[i]->y1) {
                minY = genom_[i]->y1;
            }
        }


        bmp_->clear(0, 0, 0);

        // Bounding box
        bmp_->fillRect((int) minX, (int) minY, (int) (
                maxX), (int) (maxY), 96, 0, 0);

        for (int i = 0; i < genom_len_; i++) {

            bmp_->fillRect((int) genom_[i]->x1, (int) genom_[i]->y1, (int) genom_[i]->x2, (int) genom_[i]->y2, 255, 255,
                           255);

            bmp_->drawRect((int) genom_[i]->x1, (int) genom_[i]->y1, (int) genom_[i]->x2, (int) genom_[i]->y2, 255, 0,
                           0);

            //g.drawRect((int) genom_[i].x1, (int) genom_[i].y1, (int) genom_[i].x2 - genom_[i].x1, (int) genom_[i].y2 - genom_[i].y1);
            //g.drawString(Integer.toString(i), ((genom_[i].x1 + genom_[i].x2) / 2 - strWidth / 2), (((genom_[i].y1 + genom_[i].y2) / 2) + 4));
        }

        bmp_->Write(filename);

    }

    int scWidth_, scHeight_;

    Rectangle **genom_;
    int genom_len_;

    bool init_;
    static float flip_prob_;
    static float switch_prob_;
    static float mut_prob_;
    static float mut_amount_;
    static std::shared_ptr<PopConfig> cfg_;

    static std::shared_ptr<Bitmap> bmp_;

};

std::shared_ptr<Bitmap> FpIndividual::bmp_ = 0;

float FpIndividual::flip_prob_;
float FpIndividual::switch_prob_;
float FpIndividual::mut_prob_;
float FpIndividual::mut_amount_;
std::shared_ptr<PopConfig> FpIndividual::cfg_ = 0;
