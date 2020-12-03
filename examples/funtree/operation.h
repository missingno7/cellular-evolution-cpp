#pragma once
//
// Created by MissingNO on 02.12.2020.
//

#include "expression.h"
#include <string>

class Operation : public Expression
{
public:
    Operation(__int8_t type, Expression *left, Expression *right);
    float evaluate(float x);
    Expression *clone();
    std::string toString();
    void mutate_value(Random &rnd);

    static constexpr int n_types = 4;

private:
    __int8_t _type = -1;
};

