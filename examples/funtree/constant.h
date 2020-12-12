#pragma once
//
// Created by MissingNO on 02.12.2020.
//
#include <string>

#include "expression.h"

class Constant : public Expression
        {
        public:
        Constant(float value);
        float evaluate(float x);
        Expression *clone();
        std::string toString();
        void mutate_value(Random &rnd);
        void countColor(float &col_x, float &col_y);
        bool endWithConst();

private:
            float _value;
        };

