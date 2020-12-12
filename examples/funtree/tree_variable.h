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
    void countColor(float &col_x, float &col_y);
    bool endWithConst();


protected:
    bool _sign;

};
