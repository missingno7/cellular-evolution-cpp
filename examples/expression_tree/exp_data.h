#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/bitmap.hpp"
#include <vector>
#include <fstream>
#include <sstream>


class ExpData {
public:

    std::vector<std::pair<float, float>> points;
    Bitmap bmp_;
    int scWidth, scHeight;

    ExpData() {

    }

    ExpData(PopConfig const &cfg, std::string const &points_file) {
        scWidth = 1000;
        scHeight = 1000;

        bmp_.init(scWidth, scHeight);


        // Load CSV

        // Open an existing file
        std::fstream fin;
        fin.open(points_file, std::ios::in);

        std::string line;
        float x, y;
        while (fin >> line) {
            std::vector<std::string> splitted_line = split(line, ",");
            assert(splitted_line.size() == 2);

            points.push_back(std::pair<float, float>(std::stof(splitted_line[0]), std::stof(splitted_line[1])));
        }

    }


private:

    std::vector<std::string> split(const std::string &i_str, const std::string &i_delim) {
        std::vector<std::string> result;

        size_t found = i_str.find(i_delim);
        size_t startIndex = 0;

        while (found != std::string::npos) {
            result.push_back(std::string(i_str.begin() + startIndex, i_str.begin() + found));
            startIndex = found + i_delim.size();
            found = i_str.find(i_delim, startIndex);
        }
        if (startIndex != i_str.size())
            result.push_back(std::string(i_str.begin() + startIndex, i_str.end()));
        return result;
    }
};
