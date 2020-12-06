#include <iostream>
#include "cellular-evolution/cevo/population.h"
#include "fun_individual.h"
#include "fun_data.h"


int main() {
    std::string indPath = "./";


    std::string points_path = "../data/points.csv";
    std::string config_path = "../cfg/func_config.txt";

    PopConfig cfg(config_path);

    Random rnd;

    FunData data(cfg,points_path);
    FunIndividual ind(data);

    Population<FunIndividual, FunData> pop(ind, data, cfg);

    pop.Randomize();

    FunIndividual bestOne = pop.getBest();
    std::cout << bestOne.getFitness() << std::endl;

    Bitmap bmp(data.scWidth, data.scHeight);

    while (true) {
        FunIndividual bestInd = pop.getBest();

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