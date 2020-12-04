#pragma once
//
// Created by MissingNO on 02.12.2020.
//
#include <string>

class Expression {
public:


    ~Expression();

    static Expression *makeRandom(Random &rnd, int min_depth, int max_depth, int current_depth=0);
    static void mutate(Expression *&exp, Random &rnd);
    virtual void mutate_value(Random &rnd) = 0;
    virtual float evaluate(float x) = 0;
    virtual Expression *clone() = 0;
    virtual std::string toString() = 0;
    virtual void countColor(float &col_x, float &col_y) = 0;


protected:
    Expression *_left = nullptr;
    Expression *_right = nullptr;

};