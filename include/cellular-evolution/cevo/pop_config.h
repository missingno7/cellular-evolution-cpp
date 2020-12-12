#pragma once
//
// Created by jiri on 14/11/19.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <thread>
#include <map>

class PopConfig {
public:
    PopConfig() = default;

    PopConfig(std::string filename) {
        loadConfig(filename);
    }

    void loadConfig(std::string filename) {
        std::string cvsSplitBy = " ";

        threads = std::thread::hardware_concurrency();
        std::cout << "Detected threads: " << threads << std::endl;

        std::ifstream infile(filename);
        std::string line;

        while (std::getline(infile, line)) {

            // use space as separator
            std::vector<std::string> strVals = SplitString(line, cvsSplitBy);

            if (strVals.size() < 2) {
                continue;
            }

            std::vector<std::string> new_vec;
            for (int i = 1; i < strVals.size(); i++) {
                new_vec.push_back(strVals[i]);
            }


            // add subset 1-size
            setVar(strVals[0], new_vec);
        }

    }

    bool is_present(std::string name) const{
        auto item = items_.find(name);
        if (item == items_.end()) {
            return false;
        }
        return true;
    }

    std::vector<std::string> getStr(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }
        return item->second;
    }

    std::vector<int> getInt(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }

        std::vector<int> res;

        for (std::string const &val : item->second) {
            res.push_back(std::stoi(val));
        }
        return std::move(res);
    }

    std::vector<long> getLong(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }

        std::vector<long> res;

        for (std::string const &val : item->second) {
            res.push_back(std::stol(val));
        }
        return std::move(res);
    }

    std::vector<float> getFloat(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }

        std::vector<float> res;

        for (std::string const &val : item->second) {
            res.push_back(std::stof(val));
        }
        return std::move(res);
    }

    std::vector<double> getDouble(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }

        std::vector<double> res;

        for (std::string const &val : item->second) {
            res.push_back(std::stod(val));
        }
        return std::move(res);
    }

    std::vector<bool> getBool(std::string name) const {
        auto item = items_.find(name);
        if (item == items_.end()) {
            throw std::runtime_error("Value does not exist.");
        }

        std::vector<bool> res;

        for (std::string const &val : item->second) {

            if (val == "true") {
                res.push_back(true);

            } else {
                res.push_back(false);
            }
        }
        return std::move(res);
    }

    inline int getThreads() const {
        return threads;
    }

private:
    int threads;
    std::map<std::string, std::vector<std::string>> items_;

    void setVar(std::string name, std::vector<std::string> const val) {
        auto item = items_.find(name);

        if (item == items_.end()) {
            items_.insert(std::make_pair(name, val));
        } else {
            item->second = val;
        }
    }

    std::vector<std::string> SplitString(std::string text, std::string delimiter) {
        std::istringstream iss(text);
        std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
        return std::move(results);
    }


};
