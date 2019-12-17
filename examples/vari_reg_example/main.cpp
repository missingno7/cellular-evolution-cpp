#include <iostream>
#include <fstream>
#include "cellular-evolution/varireg/varireg.h"

int main() {

    VariReg reg;
    reg.newFloat("flt");
    reg.setFloat("flt", std::vector<float>({42.5f, -10.8f}));
    std::vector<float> flt = reg.getFloat("flt");
    std::cout << flt[0] << ", " << flt[1] << std::endl;

    return 0;
}