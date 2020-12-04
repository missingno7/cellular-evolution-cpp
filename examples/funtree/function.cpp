//
// Created by jiri on 02/12/2020.
//

#include "function.h"

Function::Function(__int8_t operation_type, Expression *left) {
    _left = left;

    assert (operation_type >= 0 && operation_type < n_types);

    _type = operation_type;
}

float Function::evaluate(float x) {

    switch (_type) {
        case 0:
            return fabsf(_left->evaluate(x));
    }
}

Expression *Function::clone() {
    return new Function(_type, _left->clone());
}

std::string Function::toString() {
    switch (_type) {
        case 0:
            return "|" + _left->toString() + "|";
    }

}

void Function::mutate_value(Random &rnd)
{
    _type = rnd.nextInt(0,n_types-1);
}
