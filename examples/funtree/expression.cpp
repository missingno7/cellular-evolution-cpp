//
// Created by jiri on 02/12/2020.
//

#include "expression.h"
#include "constant.cpp"
#include "tree_variable.cpp"
#include "operation.cpp"
#include "function.cpp"

#include "cellular-evolution/utilities/random.h"

Expression::~Expression() {

    if (_left != nullptr) {
        delete _left;
    }
    if (_right != nullptr) {
        delete _right;
    }

}


Expression *Expression::makeRandom(Random &rnd, int min_depth, int max_depth, int current_depth) {


    if (current_depth < min_depth) {

        int val = rnd.nextInt(0, 1);

        switch (val) {
            case 0:
                return new Operation(rnd.nextInt(0, Operation::n_types - 1),
                                     makeRandom(rnd, min_depth, max_depth, current_depth + 1),
                                     makeRandom(rnd, min_depth, max_depth, current_depth + 1));
            case 1:
                return new Function(rnd.nextInt(0, Function::n_types - 1),
                                    makeRandom(rnd, min_depth, max_depth, current_depth + 1));

        }


    } else if (current_depth < max_depth) {

        int val = rnd.nextInt(0, 3);

        switch (val) {
            case 0:
                return new Operation(rnd.nextInt(0, Operation::n_types - 1),
                                     makeRandom(rnd, min_depth, max_depth, current_depth + 1),
                                     makeRandom(rnd, min_depth, max_depth, current_depth + 1));
            case 1:
                return new TreeVariable(rnd.nextBoolean());
            case 2:
                return new Constant(rnd.nextNormalFloat(0, 5));
            case 3:
                return new Function(rnd.nextInt(0, Function::n_types - 1),
                                    makeRandom(rnd, min_depth, max_depth, current_depth + 1));
        }

    } else {

        if (rnd.nextBoolean()) {
            return new TreeVariable(rnd.nextBoolean());
        } else {
            return new Constant(rnd.nextNormalFloat(0, 5));
        }
    }


}


void Expression::mutate(Expression *&exp, Random &rnd) {
    // Change type of current node
    float CHANGE_PROB = 0.02;
    float REPLACE_PROB = 0.02;

    // Change within same type
    float MUTATE_PROB = 0.02;

    // Switch left and right branches
    float SWITCH_PROB = 0.02;

    bool left_new = false;
    bool right_new = false;

    if (rnd.nextFloat(0.0, 1.0) < MUTATE_PROB) {
        exp->mutate_value(rnd);
    }

    if (exp->_left != nullptr && exp->_right != nullptr && rnd.nextFloat(0.0, 1.0) < SWITCH_PROB) {
        Expression *tmp = exp->_right;
        exp->_right = exp->_left;
        exp->_left = tmp;
    }

    if (rnd.nextFloat(0.0, 1.0) < CHANGE_PROB) {

        Expression *left = exp->_left;
        exp->_left = nullptr;
        Expression *right = exp->_right;
        exp->_right = nullptr;
        delete exp;

        int val = rnd.nextInt(0, 3);

        switch (val) {

            // Operation
            case 0: {
                if (left == nullptr) {
                    left = makeRandom(rnd, 0, 4, 0);
                    left_new = true;
                }

                if (right == nullptr) {
                    right = makeRandom(rnd, 0, 4, 0);
                    right_new = true;
                }


                exp = new Operation(rnd.nextInt(0, Operation::n_types - 1),
                                    left,
                                    right);
                break;
            }

                // Variable
            case 1: {
                if (left != nullptr) {
                    delete left;
                }
                if (right != nullptr) {
                    delete right;
                }
                exp = new TreeVariable(rnd.nextBoolean());

                break;
            }

                // Constant
            case 2: {
                if (left != nullptr) {
                    delete left;
                }
                if (right != nullptr) {
                    delete right;
                }
                exp = new Constant(rnd.nextNormalFloat(0, 5));

                break;
            }

                // Function
            case 3: {
                Expression *new_node;
                if (left == nullptr) {
                    if (right == nullptr) {
                        // Both null = make new
                        new_node = makeRandom(rnd, 0, 4, 0);
                        left_new = true;
                    } else {
                        // Only right not null - use right
                        new_node = right;
                    }
                } else {
                    if (right == nullptr) {
                        // Only left not null - use left
                        new_node = left;
                    } else {
                        // both not null, use one, delete other
                        if (rnd.nextBoolean()) {
                            new_node = right;
                            delete left;
                        } else {
                            new_node = left;
                            delete right;
                        }
                    }
                }

                exp = new Function(rnd.nextInt(0, Function::n_types - 1),
                                   new_node);
                break;
            }
        }
    }


    if (rnd.nextFloat(0.0, 1.0) < REPLACE_PROB) {

        if (!left_new && exp->_left != nullptr && rnd.nextBoolean()) {
            delete (exp->_left);
            exp->_left = makeRandom(rnd, 0, 4, 0);
        }

        if (!right_new && exp->_right != nullptr && rnd.nextBoolean()) {
            delete (exp->_right);
            exp->_right = makeRandom(rnd, 0, 4, 0);
        }
    }


    if (exp->_left != nullptr && !left_new) {
        mutate(exp->_left, rnd);
    }

    if (exp->_right != nullptr && !right_new) {
        mutate(exp->_left, rnd);
    }

}