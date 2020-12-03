#include <iostream>
#include "cellular-evolution/cevo/population.h"
#include "expression.cpp"

int main() {

    Random rnd;

    //Expression *exp = new Operation(0, new TreeVariable(rnd.nextBoolean()), new TreeVariable(rnd.nextBoolean()));
    Expression *exp = Expression::makeRandom(rnd,2,5);

    std::cout<< exp->toString()<<std::endl;
    std::cout<< exp->evaluate(1)<<std::endl;

    for(int i=0 ; i<100 ; i++)
    {
    Expression::mutate(exp,rnd);

    std::cout<< exp->toString()<<std::endl;
    std::cout<< exp->evaluate(1)<<std::endl;;
    }

}