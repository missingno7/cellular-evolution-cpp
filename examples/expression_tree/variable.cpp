//
// Created by jiri on 02/12/2020.
//

#include "variable.h"
#include <string>

Variable::Variable(bool sign) {
    _sign = sign;
}

float Variable::evaluate(float x) {
    if (_sign) {
        return x;
    } else {
        return -x;
    }
}

Expression *Variable::clone() {
    return new Variable(_sign);
}

std::string Variable::toString() {
    if (_sign) {
        return "-x";
    } else {
        return "x";
    }
}

void Variable::mutate_value(Random &rnd) {
}

void Variable::countColor(float &col_x, float &col_y) {
    if (_sign) {
        col_y += 1.0;
    } else {
        col_y -= 1.0;
    }
}

bool Variable::endWithConst()
{
    return false;
}
