//
// Created by jiri on 04/12/2020.
//
#include<iostream>
#include"cellular-evolution/utilities/bitmap.hpp"
#include"cellular-evolution/utilities/random.h"


int main() {

    //Bitmap bmp("../data/tst.jpg");
    //bmp.Write("out.bmp");

    Bitmap bmp(512,512);
    Random rnd;

    bmp.fillEllipse(10,10,500,500,rnd.nextInt(0,255),rnd.nextInt(0,255),rnd.nextInt(0,255));

    bmp.Write("out.bmp");

}