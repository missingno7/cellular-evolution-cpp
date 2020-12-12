#pragma once
//
// Created by MissingNO on 02.12.2020.
//

#include "expression.h"
#include <string>

class Function : public Expression
{
public:
    Function(__int8_t type, Expression *left);
    float evaluate(float x);
    Expression *clone();
    std::string toString();
    void mutate_value(Random &rnd);
    void countColor(float &col_x, float &col_y);
    bool endWithConst();

    static constexpr int n_types = 1;

private:
    __int8_t _type = -1;
};

