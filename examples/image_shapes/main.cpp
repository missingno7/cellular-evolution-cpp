//
// Created by jiri on 04/12/2020.
//
#include<iostream>
#include"cellular-evolution/utilities/bitmap.hpp"
#include"cellular-evolution/utilities/random.h"


int main() {

    //Bitmap bmp("../data/tst.jpg");
    //bmp.Write("out.bmp");

    Bitmap bmp(1024,1024);
    Random rnd;

    bmp.fillEllipseXY(10,10,1000,1000,rnd.nextInt(0,255),rnd.nextInt(0,255),rnd.nextInt(0,255));



    bmp.fillEllipseHole(500,500,200,200,100,150,255,255,0);

    bmp.Write("out.bmp");

}