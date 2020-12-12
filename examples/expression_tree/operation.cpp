//
// Created by jiri on 02/12/2020.
//

#include "operation.h"


Operation::Operation(__int8_t operation_type, Expression *left, Expression *right) {
    _left = left;
    _right = right;

    assert (operation_type >= 0 && operation_type < n_types);

    _type = operation_type;
}

float Operation::evaluate(float x) {

    switch (_type) {
        case 0:
            return _left->evaluate(x) + _right->evaluate(x);
        case 1:
            return _left->evaluate(x) - _right->evaluate(x);
        case 2:
            return _left->evaluate(x) * _right->evaluate(x);
        case 3:
            return _left->evaluate(x) / _right->evaluate(x);
        case 4:
            return powf(_left->evaluate(x), _right->evaluate(x));
    }
}

Expression *Operation::clone() {
    return new Operation(_type, _left->clone(), _right->clone());
}

std::string Operation::toString() {
    switch (_type) {
        case 0:
            return "(" + _left->toString() + "+" + _right->toString() + ")";
        case 1:
            return "(" + _left->toString() + "-" + _right->toString() + ")";
        case 2:
            return "(" + _left->toString() + "*" + _right->toString() + ")";
        case 3:
            return "(" + _left->toString() + "/" + _right->toString() + ")";
        case 4:
            return "(" + _left->toString() + "^" + _right->toString() + ")";
    }

}

void Operation::mutate_value(Random &rnd)
{
    _type = rnd.nextInt(0,n_types-1);
}

void Operation::countColor(float &col_x, float &col_y) {
    col_y += float(_type)-(float(n_types)/2.0);

    _left->countColor(col_x,col_y);
    _right->countColor(col_x,col_y);

}

bool Operation::endWithConst()
{
    return _left->endWithConst() && _right->endWithConst();
}
