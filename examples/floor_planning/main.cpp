#include <iostream>
#include <fstream>
#include "cellular-evolution/floor_planning/fp_individual.h"
#include "cellular-evolution/cevo/population.h"

int main() {

    std::string indPath = "out\\";

    std::shared_ptr<PopConfig> cfg = std::make_shared<PopConfig>();

    // Custom parameter
    cfg->reg.newInt("squares");
    cfg->reg.newFloat("flipprob");
    cfg->reg.newFloat("switchprob");

    cfg->LoadConfig("cell_config.txt");

    std::shared_ptr<FpData> fpData = std::make_shared<FpData>("squares.txt", cfg);
    std::shared_ptr<IndData> tstData = fpData;
    int perfectFitness = fpData->getPerfection();

    std::shared_ptr<Individual> tstInd = std::make_shared<FpIndividual>(fpData->scWidth, fpData->scHeight, fpData->squares,cfg);

    Population pop(tstInd, tstData, cfg);

    pop.Randomize(Random());

    FpIndividual *bestOne = dynamic_cast<FpIndividual*>(pop.getBest());


    while (true) {
        FpIndividual *bestInd =  dynamic_cast<FpIndividual*>(pop.getBest());

        if (bestInd->getFitness() > bestOne->getFitness()) {
            bestOne = dynamic_cast<FpIndividual*>(bestInd->clone());
            bestInd->Draw(tstData, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (cfg->drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
        }

        std::cout<<"GENERATION " << pop.getGen()<<std::endl;
        std::cout<<bestInd->toString(tstData)<<std::endl;
        std::cout<<"GEN BEST FITNESS: " << std::to_string((perfectFitness * 100) / - (bestInd->getFitness())) << " %"<<std::endl;
        std::cout<<"AVG FITNESS: " << std::to_string( (perfectFitness * 100 / -pop.avgFitness())) << " %"<<std::endl;
        std::cout<<"ALLBEST FITNESS: " <<  std::to_string(perfectFitness * 100 / -bestOne->getFitness()) << " %"<<std::endl;

        pop.nextGen();
    }
}