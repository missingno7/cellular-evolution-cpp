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


private:
            float _value;
        };

