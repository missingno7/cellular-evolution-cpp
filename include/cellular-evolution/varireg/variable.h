#pragma once
////
// Created by jiri on 14/11/19.
//

#include<vector>

enum VariableType {
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    BOOL
};

class Variable {
public:
    Variable() {

        has_val_ = false;
    }

    virtual ~Variable() = default;

    VariableType type_;
    bool has_val_;
};

class VBool : public Variable {
public:
    VBool() : Variable() {
        type_ = VariableType::BOOL;
    }

    void setVal(std::vector<bool> const &val) {
        val_ = val;
        this->has_val_ = true;
    }

    std::vector<bool> getVal() {
        if (!this->has_val_)throw std::runtime_error("Value was not assigned.");
        return val_;
    }

    std::vector<bool> val_;
};

class VInteger : public Variable {
public:
    VInteger() : Variable() {
        type_ = VariableType::INT;
    }

    void setVal(std::vector<int> const &val) {
        val_ = val;
        this->has_val_ = true;
    }

    std::vector<int> getVal() {
        if (!this->has_val_)throw std::runtime_error("Value was not assigned.");
        return val_;
    }

    std::vector<int> val_;
};


class VFloat : public Variable {
public:
    VFloat() : Variable() {
        type_ = VariableType::FLOAT;
    }

    void setVal(std::vector<float> const &val) {
        val_ = val;
        this->has_val_ = true;
    }

    std::vector<float> getVal() {
        if (!this->has_val_)throw std::runtime_error("Value was not assigned.");
        return val_;
    }

    std::vector<float> val_;
};

class VDouble : public Variable {
public:
    VDouble() : Variable() {
        type_ = VariableType::DOUBLE;
    }

    void setVal(std::vector<double> const &val) {
        val_ = val;
        this->has_val_ = true;
    }

    std::vector<double> getVal() {
        if (!this->has_val_)throw std::runtime_error("Value was not assigned.");
        return val_;
    }

    std::vector<double> val_;
};

class VLong : public Variable {
public:
    VLong() : Variable() {
        type_ = VariableType::LONG;
    }

    void setVal(std::vector<long> const &val) {
        val_ = val;
        this->has_val_ = true;
    }

    std::vector<long> getVal() {
        if (!this->has_val_)throw std::runtime_error("Value was not assigned.");
        return val_;
    }

    std::vector<long> val_;
};