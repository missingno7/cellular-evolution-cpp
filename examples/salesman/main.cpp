#include <iostream>
#include <fstream>
#include "sa_data.h"
#include "s_individual.h"

#include "cellular-evolution/cevo/population.h"


int main() {


    std::string indPath = "out\\";

    std::shared_ptr<PopConfig> cfg = std::make_shared<PopConfig>();

    // Custom parameter
    cfg->reg.newInt("cities");
    cfg->reg.newInt("scwidth");
    cfg->reg.newInt("scheight");
    cfg->reg.newFloat("revprob");
    cfg->reg.newFloat("shiftprob");

    cfg->LoadConfig("../cfg/cities_config.txt");

    std::shared_ptr<SaData> sa_data = std::make_shared<SaData>(cfg);
    std::shared_ptr<IndData> tstData = sa_data;

    std::shared_ptr<SaIndividual> tstInd = std::make_shared<SaIndividual>(sa_data->scWidth_, sa_data->scHeight_,
                                                                          sa_data->cities_, cfg);

    Population<SaIndividual> pop(tstInd, tstData, cfg);

    pop.Randomize();

    std::shared_ptr<SaIndividual> bestOne = pop.getBest()->clone();


    while (true) {
        std::shared_ptr<SaIndividual> bestInd = pop.getBest()->clone();

        if (bestInd->getFitness() > bestOne->getFitness()) {
            bestOne = bestInd;
            bestInd->Draw(tstData, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (cfg->drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
        }

        std::cout << "GENERATION " << pop.getGen() << std::endl;
        std::cout << bestInd->toString(tstData) << std::endl;
        std::cout << "GEN BEST FITNESS: " << bestInd->getFitness() << std::endl;
        std::cout << "AVG FITNESS: " << pop.avgFitness() << std::endl;
        std::cout << "ALLBEST FITNESS: " << bestOne->getFitness() << std::endl;

        pop.nextGen();
    }
}