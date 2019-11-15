#pragma once
////
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/varireg/variable.h"
#include <string>
#include <map>
#include <iostream>
#include <memory>

class VariReg {
public:
    VariReg() = default;

    void newInt(std::string name) {
        items_.insert(std::make_pair(name, std::make_shared<VInteger>()));
    }

    void newLong(std::string name) {
        items_.insert(std::make_pair(name, std::make_shared<VLong>()));
    }

    void newFloat(std::string name) {
        items_.insert(std::make_pair(name, std::make_shared<VFloat>()));
    }

    void newDouble(std::string name) {
        items_.insert(std::make_pair(name, std::make_shared<VDouble>()));
    }

    void newBool(std::string name) {
        items_.insert(std::make_pair(name, std::make_shared<VBool>()));
    }


    std::shared_ptr<Variable> getVar(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return item->second;
    }


    void setInt(std::string name, std::vector<int> const val) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        std::dynamic_pointer_cast<VInteger>(item->second)->setVal(val);
    }

    void setBool(std::string name, std::vector<bool> const val) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        std::dynamic_pointer_cast<VBool>(item->second)->setVal(val);
    }

    void setFloat(std::string name, std::vector<float> const val) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        std::dynamic_pointer_cast<VFloat>(item->second)->setVal(val);
    }

    void setDouble(std::string name, std::vector<double> &val) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        std::dynamic_pointer_cast<VDouble>(item->second)->setVal(val);
    }

    void setLong(std::string name, std::vector<long> &val) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        std::dynamic_pointer_cast<VLong>(item->second)->setVal(val);
    }

    std::vector<int> getInt(std::string name) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return std::dynamic_pointer_cast<VInteger>(item->second)->getVal();
    }

    std::vector<bool> getBool(std::string name) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return std::dynamic_pointer_cast<VBool>(item->second)->getVal();
    }

    std::vector<float> getFloat(std::string name) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return std::dynamic_pointer_cast<VFloat>(item->second)->getVal();
    }

    std::vector<double> getDouble(std::string name) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return std::dynamic_pointer_cast<VDouble>(item->second)->getVal();
    }

    std::vector<long> getLong(std::string name) {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return std::dynamic_pointer_cast<VLong>(item->second)->getVal();
    }


    std::map<std::string, std::shared_ptr<Variable>> items_;

};