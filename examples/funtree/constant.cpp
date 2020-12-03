//
// Created by jiri on 02/12/2020.
//

#include "constant.h"
#include <string>

Constant::Constant(float value) {
    _value = value;
}

float Constant::evaluate(float x) {
    return _value;
}

Expression *Constant::clone() {
    return new Constant(_value);
}

std::string Constant::toString() {
    return std::to_string(_value);
}

void Constant::mutate_value(Random &rnd)
{
 _value += rnd.nextFloat(-1,1);
}
