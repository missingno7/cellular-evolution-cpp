#pragma once
//
// Created by MissingNO on 02.12.2020.
//

#include <expression.h>

class Constant : public Expression
        {
        public:

        Constant(float value)
        {
            _value=value;
        }

        private:
            float _value;
        };

