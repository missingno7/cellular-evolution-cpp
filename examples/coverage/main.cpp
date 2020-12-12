//
// Created by jiri on 12/12/2020.
//

#include <iostream>
#include <algorithm>

#include "cellular-evolution/utilities/random.h"

#include "coverage_data.h"
#include "coverage_ind.h"
#include "cellular-evolution/cevo/population.h"


int main() {


    Random rnd;

    std::string indPath = "./";

    PopConfig cfg("../cfg/coverage_config.txt");

    CoverageData data(rnd, cfg);

    CoverageIndividual template_ind(data);


    Population<CoverageIndividual, CoverageData> pop(template_ind, data, cfg);

    pop.Randomize();

    CoverageIndividual bestOne = pop.getWorst();




    while (true) {
        CoverageIndividual bestInd = pop.getBest();

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


    return 0;
}