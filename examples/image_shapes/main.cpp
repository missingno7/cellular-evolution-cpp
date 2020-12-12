//
// Created by jiri on 04/12/2020.
//
#include<iostream>
#include"cellular-evolution/utilities/bitmap.hpp"
#include"cellular-evolution/utilities/random.h"
#include "cellular-evolution/cevo/population.h"
#include "shape.h"
#include "img_data.h"
#include "img_individual.h"


int main() {

    std::string indPath = "./";

    std::string bmp_filename("../data/tst.jpg");

    PopConfig cfg("../cfg/img_config.txt");
    ImgData data(cfg, bmp_filename);

    Random rnd;

    ImgIndividual ind(rnd,data);

    Population<ImgIndividual, ImgData> pop(ind, data, cfg);

    pop.Randomize();

    ImgIndividual bestOne = pop.getBest();
    std::cout << bestOne.getFitness() << std::endl;

    Bitmap bmp(data._sc_width, data._sc_height);

    while (true) {
        ImgIndividual bestInd = pop.getBest();

       if (bestInd.getFitness() > bestOne.getFitness()) {
            bestOne = bestInd;
            bestInd.Draw(data, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (pop.m_drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
       }

        std::cout << "GENERATION " << pop.getGen() << std::endl;
        std::cout << bestInd.toString(data) << std::endl;
        std::cout << "GEN BEST FITNESS: " << bestInd.getFitness() << std::endl;
        std::cout << "AVG FITNESS: " << pop.avgFitness() << std::endl;
        std::cout << "ALLBEST FITNESS: " << bestOne.getFitness() << std::endl;

        pop.nextGen();
    }


}