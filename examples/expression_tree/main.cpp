#include <iostream>
#include "cellular-evolution/cevo/population.h"
#include "exp_individual.h"
#include "exp_data.h"


int main() {
    std::string indPath = "./";


    std::string points_path = "../data/points.csv";
    std::string config_path = "../cfg/func_config.txt";

    PopConfig cfg(config_path);

    Random rnd;

    ExpData data(cfg,points_path);
    ExpIndividual ind(data);

    Population<ExpIndividual, ExpData> pop(ind, data, cfg);

    pop.Randomize();

    ExpIndividual bestOne = pop.getBest();
    std::cout << bestOne.getFitness() << std::endl;

    Bitmap bmp(data.scWidth, data.scHeight);

    while (true) {
        ExpIndividual bestInd = pop.getBest();

        if (bestInd.getFitness() > bestOne.getFitness()) {
            bestOne = bestInd;
            bestInd.Draw(data, indPath + "IGEN" + std::to_string(pop.getGen()) + ".bmp");

            if (pop.m_drawpop) {
                pop.paintPop(indPath + "GEN" + std::to_string(pop.getGen()) + ".bmp");
            }
        }

        std::cout << "GENERATION " << pop.getGen() << std::endl;
        bestInd.simplify();
        std::cout << bestInd.toString(data) << std::endl;
        std::cout << "GEN BEST FITNESS: " << bestInd.getFitness() << std::endl;
        std::cout << "AVG FITNESS: " << pop.avgFitness() << std::endl;
        std::cout << "ALLBEST FITNESS: " << bestOne.getFitness() << std::endl;

        pop.nextGen();
    }



}