#include <iostream>
#include <fstream>
#include "cellular-evolution/utilities/bitmap.hpp"
#include "cellular-evolution/cevo/population.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "test_ind.h"
#include <memory>

int main() {
    std::string indPath = "out/";

    std::shared_ptr<PopConfig> cfg = std::make_shared<PopConfig>();


    cfg->reg.newInt("tstint");
    cfg->LoadConfig("config.txt");
    std::vector<int> tstint = cfg->reg.getInt("tstint");
    std::cout << tstint.size() << " - " << tstint[1] << std::endl;

    std::shared_ptr<TestInd> tstInd = std::make_shared<TestInd>();
    std::shared_ptr<IndData> tstData = std::make_shared<TestIndData>();

    Population<TestInd> pop(tstInd, tstData, cfg);
    pop.Randomize();

    for (int i = 0; i < 100; i++) {
        TestInd *bestInd = pop.getBest();
        std::cout << "GENERATION " << std::to_string(pop.getGen()) << std::endl;
        std::cout << bestInd->toString(tstData) << std::endl;
        std::cout << "BEST FITNESS: " << std::to_string(bestInd->getFitness()) << std::endl;
        std::cout << "AVG FITNESS: " << std::to_string(pop.avgFitness()) << std::endl;
        if (cfg->drawpop)pop.paintPop(indPath + "GEN" + std::to_string(i) + ".bmp");
        pop.nextGen();
    }

    return 0;
}