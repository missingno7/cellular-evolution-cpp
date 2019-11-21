#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/cevo/pop_config.h"

class FpData : public IndData {
public:

int *width;
int *height;
int scWidth;
int scHeight;
int squares;

    ~FpData()
    {
        delete []width;
        delete []height;
    }

FpData(std::string filename, std::shared_ptr<PopConfig> cfg) {
scWidth = 1000;
scHeight = 1000;

squares = cfg->reg.getInt("squares")[0];

width = new int[squares];
height = new int[squares];
Random rnd;

for (int i = 0; i < squares; i++) {
width[i] = 10 + std::abs(rnd.nextInt(0,100));
height[i] = 10 + std::abs(rnd.nextInt(0,100));
}
/*for(int i=0;i<squares;i++){
    width[i]=5+rnd.nextInt()%15;
    height[i]=5+rnd.nextInt()%15;
}*/

/*
width[0]=10;
height[0]=20;

width[1]=10;
height[1]=50;

width[2]=20;
height[2]=30;

width[3]=40;
height[3]=10;

width[4]=30;
height[4]=20;

width[5]=30;
height[5]=10;

width[6]=40;
height[6]=10;

width[7]=40;
height[7]=20;

width[8]=20;
height[8]=10;

width[9]=20;
height[9]=30;

width[10]=10;
height[10]=30;

width[11]=10;
height[11]=20;

width[12]=10;
height[12]=50;

width[13]=20;
height[13]=30;

width[14]=40;
height[14]=10;

width[15]=30;
height[15]=20;

width[16]=30;
height[16]=10;

width[17]=40;
height[17]=10;

width[18]=40;
height[18]=20;

width[19]=20;
height[19]=10;

width[20]=20;
height[20]=30;

width[21]=10;
height[21]=30;
*/

/*for (int i = 0; i < squares; i++) {
           width[i] = 50;
           height[i] = 50;
       }*/
}

int getPerfection() {
    int sum = 0;
    for (int i = 0; i < squares ; i++) {
        sum += width[i] * height[i];
    }

    return sum;
}

};
