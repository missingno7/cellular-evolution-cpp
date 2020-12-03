#pragma once
//
// Created by jiri on 02/12/2020.
//

#include "expression.h"
#include <string>


class TreeVariable : public Expression {
public:

    TreeVariable(bool sign);

    float evaluate(float x);

    Expression *clone();

    std::string toString();

    void mutate_value(Random &rnd);

protected:
    bool _sign;

};
