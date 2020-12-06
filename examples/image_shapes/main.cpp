//
// Created by jiri on 04/12/2020.
//
#include<iostream>
#include"cellular-evolution/utilities/bitmap.hpp"
#include"cellular-evolution/utilities/random.h"
#include "shape.h"
#include "img_data.h"
#include "img_individual.h"


int main() {

    std::string bmp_filename("../data/tst.jpg");

    PopConfig cfg("../cfg/img_config.txt");
    ImgData data(cfg, bmp_filename);

    Random rnd;

    ImgIndividual ind(rnd,data);


}