#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/varireg/varireg.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <thread>

class PopConfig {
public:
    PopConfig() = default;

    std::vector<std::string> SplitString(std::string text, std::string delimiter) {
        std::istringstream iss(text);
        std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
        return std::move(results);
    }


    void LoadConfig(std::string filename) {
        std::string cvsSplitBy = " ";

        threads = std::thread::hardware_concurrency();
        std::cout << "Detected threads: " << threads << std::endl;

        std::ifstream infile(filename);
        std::string line;

        while (std::getline(infile, line)) {

            // use comma as separator
            std::vector<std::string> strVals = SplitString(line, cvsSplitBy);

            if (strVals.size() < 2) {
                continue;
            }

            if (strVals[0] == "xpopsize") {
                xpopsize = std::stoi(strVals[1]);
            } else if (strVals[0] == "ypopsize") {
                ypopsize = std::stoi(strVals[1]);
            } else if (strVals[0] == "threads") {
                threads = std::stoi(strVals[1]);
            } else if (strVals[0] == "crossrate") {
                crossrate = std::stof(strVals[1]);
            } else if (strVals[0] == "mutamount") {
                mutamount = std::stof(strVals[1]);
            } else if (strVals[0] == "mutprob") {
                mutprob = std::stof(strVals[1]);
            } else if (strVals[0] == "mutprob") {
                if (strVals[1] == "true") {
                    mutprob = true;
                } else {
                    mutprob = false;
                }
            } else if (strVals[0] == "gennew") {
                if (strVals[1] == "true") {
                    gennew = true;
                } else {
                    gennew = false;
                }
            } else if (strVals[0] == "drawpop") {
                if (strVals[1] == "true") {
                    drawpop = true;
                } else {
                    drawpop = false;
                }
            } else {

                std::shared_ptr<Variable> var = reg.getVar(strVals[0]);

                if (var) {
                    if (strVals.size() < 2) {
                        throw std::runtime_error(
                                "Variabla " + strVals[0] + " has bad size " + std::to_string(strVals.size()));
                    }
                    switch (var->type_) {
                        case INT: {
                            std::vector<int> iVals;
                            for (int i = 1; i < strVals.size(); i++) {
                                iVals.push_back(std::stoi(strVals[i]));
                            }
                            reg.setInt(strVals[0], iVals);
                            break;
                        }
                        case FLOAT: {
                            std::vector<float> fVals;
                            for (int i = 1; i < strVals.size(); i++) {
                                fVals.push_back(std::stof(strVals[i]));
                            }
                            reg.setFloat(strVals[0], fVals);
                            break;
                        }
                        case DOUBLE: {
                            std::vector<double> dVals;
                            for (int i = 1; i < strVals.size(); i++) {
                                dVals.push_back(std::stod(strVals[i]));
                            }
                            reg.setDouble(strVals[0], dVals);
                            break;
                        }
                        case LONG: {
                            std::vector<long> lVals;
                            for (int i = 1; i < strVals.size(); i++) {
                                lVals.push_back(std::stol(strVals[i]));
                            }
                            reg.setLong(strVals[0], lVals);
                            break;
                        }
                        case BOOL: {
                            std::vector<bool> bVals;
                            bool val;
                            for (int i = 1; i < strVals.size(); i++) {
                                if (strVals[i] == "true") {
                                    val = true;
                                } else {
                                    val = false;
                                }
                                bVals.push_back(val);
                            }
                            reg.setBool(strVals[0], bVals);
                            break;
                        }
                    }
                }
            }
        }
    }

public:
    VariReg reg;
    int xpopsize = 512;
    int ypopsize = 512;
    int tsize = 0;
    float mutamount = 0.1;
    float mutprob = 1.0;
    float crossrate = 0.5;
    int threads = 16;
    bool gennew = true;
    bool drawpop = true;

};
