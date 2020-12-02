#pragma once
//
// Created by MissingNO on 02.12.2020.
//

#include <constant.h>

class Expression {
public:


    ~Expression() {
        if (left != nullptr) {
            delete left;
        }
        if (right != nullptr) {
            delete right;
        }
    }


    static Expression *make_random() {
        return new Constant();
    }

    Expression *left = nullptr;
    Expression *right = nullptr;

};