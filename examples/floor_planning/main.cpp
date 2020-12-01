#include <iostream>
#include "fp_individual.h"
#include "cellular-evolution/cevo/population.h"

int main() {

    std::string indPath = "./";

    PopConfig cfg;

    // Custom parameter
    cfg.reg.newInt("squares");
    cfg.reg.newFloat("flipprob");
    cfg.reg.newFloat("switchprob");

    cfg.LoadConfig("../cfg/cell_config.txt");

    FpData sa_data(cfg);
    int perfectFitness = sa_data.getPerfection();

    FpIndividual tstInd(sa_data);

    Population<FpIndividual, FpData> pop(tstInd, sa_data, cfg);

    pop.Randomize();

    FpIndividual bestOne = pop.getBest();


    while (true) {
        FpIndividual bestInd = pop.getBest();

        if (bestInd.getFitness() > bestOne.getFitness()) {
            bestOne = bestInd;
            bestInd.Draw(sa_data, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (cfg.drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
        }

        std::cout << "GENERATION " << pop.getGen() << std::endl;
        std::cout << bestInd.toString(sa_data) << std::endl;
        std::cout << "GEN BEST FITNESS: " << std::to_string((perfectFitness * 100) / -(bestInd.getFitness())) << " %"
                  << std::endl;
        std::cout << "AVG FITNESS: " << std::to_string((perfectFitness * 100 / -pop.avgFitness())) << " %" << std::endl;
        std::cout << "ALLBEST FITNESS: " << std::to_string(perfectFitness * 100 / -bestOne.getFitness()) << " %"
                  << std::endl;

        pop.nextGen();
    }
}