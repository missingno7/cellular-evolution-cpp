#include <iostream>
#include <fstream>
#include "sa_data.h"
#include "s_individual.h"

#include "cellular-evolution/cevo/population.h"


int main() {


    std::string indPath = "./";

    PopConfig cfg;

    // Custom parameter
    cfg.reg.newInt("cities");
    cfg.reg.newInt("scwidth");
    cfg.reg.newInt("scheight");
    cfg.reg.newFloat("revprob");
    cfg.reg.newFloat("shiftprob");

    cfg.LoadConfig("../cfg/cities_config.txt");

    SaData sa_data(cfg);
    SaIndividual tstInd(sa_data);

    Population<SaIndividual, SaData> pop(tstInd, sa_data, cfg);

    pop.Randomize();

    SaIndividual bestOne = pop.getBest();
    std::cout << bestOne.fitness << std::endl;

    Bitmap bmp(sa_data.scWidth_, sa_data.scHeight_);

    while (true) {
        SaIndividual bestInd = pop.getBest();

        if (bestInd.getFitness() > bestOne.getFitness()) {
            bestOne = bestInd;
            bestInd.Draw(sa_data, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (cfg.drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
        }

        std::cout << "GENERATION " << pop.getGen() << std::endl;
        std::cout << bestInd.toString(sa_data) << std::endl;
        std::cout << "GEN BEST FITNESS: " << bestInd.getFitness() << std::endl;
        std::cout << "AVG FITNESS: " << pop.avgFitness() << std::endl;
        std::cout << "ALLBEST FITNESS: " << bestOne.getFitness() << std::endl;

        pop.nextGen();
    }
}