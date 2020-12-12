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
        case 1:
            return tanf(_left->evaluate(x));
        case 2:
            return sinf(_left->evaluate(x));
    }
}

Expression *Function::clone() {
    return new Function(_type, _left->clone());
}

std::string Function::toString() {
    switch (_type) {
        case 0:
            return "|" + _left->toString() + "|";
        case 1:
            return "tan(" + _left->toString() + ")";
        case 2:
            return "sin(" + _left->toString() + ")";
    }

}

void Function::mutate_value(Random &rnd)
{
    _type = rnd.nextInt(0,n_types-1);
}

void Function::countColor(float &col_x, float &col_y) {
    col_y += float(_type)-float(n_types)/2.0;
    _left->countColor(col_x,col_y);
}

bool Function::endWithConst()
{
    return _left->endWithConst();
}
