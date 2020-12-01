#include <iostream>
#include "cellular-evolution/cevo/population.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "test_ind.h"
#include <memory>

int main() {
    std::string indPath = "./";

    PopConfig cfg;


    cfg.reg.newInt("tstint");
    cfg.LoadConfig("../cfg/tst_config.txt");
    std::vector<int> tstint = cfg.reg.getInt("tstint");
    std::cout << tstint.size() << " - " << tstint[1] << std::endl;

    TestIndData tst_data;
    TestInd tst_ind(tst_data);

    Population<TestInd, TestIndData> pop(tst_ind, tst_data, cfg);
    pop.Randomize();

    for (int i = 0; i < 100; i++) {
        TestInd bestInd = pop.getBest();
        std::cout << "GENERATION " << std::to_string(pop.getGen()) << std::endl;
        std::cout << bestInd.toString(tst_data) << std::endl;
        std::cout << "BEST FITNESS: " << std::to_string(bestInd.getFitness()) << std::endl;
        std::cout << "AVG FITNESS: " << std::to_string(pop.avgFitness()) << std::endl;
        if (cfg.drawpop)pop.paintPop(indPath + "GEN" + std::to_string(i) + ".bmp");
        pop.nextGen();
    }

    return 0;
}