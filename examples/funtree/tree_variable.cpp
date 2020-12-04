//
// Created by jiri on 02/12/2020.
//

#include "tree_variable.h"
#include <string>

TreeVariable::TreeVariable(bool sign) {
    _sign = sign;
}

float TreeVariable::evaluate(float x) {
    if (_sign) {
        return x;
    } else {
        return -x;
    }
}

Expression *TreeVariable::clone() {
    return new TreeVariable(_sign);
}

std::string TreeVariable::toString() {
    if (_sign) {
        return "-x";
    } else {
        return "x";
    }
}

void TreeVariable::mutate_value(Random &rnd) {
}

void TreeVariable::countColor(float &col_x, float &col_y) {
    if (_sign) {
        col_y += 1.0;
    } else {
        col_y -= 1.0;
    }
}