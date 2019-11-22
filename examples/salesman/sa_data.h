#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/cevo/ind_data.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/random.h"

class SaData : public IndData {
public:
    float *x;
    float *y;
    int cities_;
    int scWidth_;
    int scHeight_;

    SaData(std::shared_ptr<PopConfig> cfg) {
        scWidth_ = cfg->reg.getInt("scwidth")[0];
        scHeight_ = cfg->reg.getInt("scheight")[0];

        cities_ = cfg->reg.getInt("cities")[0];
        //int cities = 60;
        x = new float[cities_];
        y = new float[cities_];

        /*
        x[0] = 12;
        y[0] = 84;
        x[1] = 118;
        y[1] = 164;
        x[2] = 179;
        y[2] = 178;
        x[3] = 189;
        y[3] = 148;
        x[4] = 17;
        y[4] = 152;
        x[5] = 108;
        y[5] = 129;
        x[6] = 51;
        y[6] = 161;
        x[7] = 165;
        y[7] = 149;
        x[8] = 170;
        y[8] = 90;
        x[9] = 73;
        y[9] = 118;
        x[10] = 104;
        y[10] = 18;
        x[11] = 71;
        y[11] = 77;
        x[12] = 108;
        y[12] = 53;
        x[13] = 165;
        y[13] = 86;
        x[14] = 6;
        y[14] = 30;
        x[15] = 130;
        y[15] = 54;
        x[16] = 52;
        y[16] = 108;
        x[17] = 79;
        y[17] = 52;
        x[18] = 183;
        y[18] = 179;
        x[19] = 188;
        y[19] = 64;
        x[20] = 34;
        y[20] = 177;
        x[21] = 152;
        y[21] = 52;
        x[22] = 191;
        y[22] = 178;
        x[23] = 133;
        y[23] = 52;
        x[24] = 152;
        y[24] = 109;
        x[25] = 182;
        y[25] = 141;
        x[26] = 47;
        y[26] = 63;
        x[27] = 137;
        y[27] = 63;
        x[28] = 61;
        y[28] = 159;
        x[29] = 15;
        y[29] = 159;
        x[30] = 24;
        y[30] = 157;
        x[31] = 111;
        y[31] = 79;
        x[32] = 124;
        y[32] = 164;
        x[33] = 23;
        y[33] = 49;
        x[34] = 62;
        y[34] = 128;
        x[35] = 194;
        y[35] = 14;
        x[36] = 145;
        y[36] = 151;
        x[37] = 161;
        y[37] = 158;
        x[38] = 61;
        y[38] = 147;
        x[39] = 122;
        y[39] = 28;
        x[40] = 99;
        y[40] = 107;
        x[41] = 102;
        y[41] = 149;
        x[42] = 193;
        y[42] = 171;
        x[43] = 179;
        y[43] = 48;
        x[44] = 16;
        y[44] = 165;
        x[45] = 105;
        y[45] = 105;
        x[46] = 156;
        y[46] = 81;
        x[47] = 60;
        y[47] = 118;
        x[48] = 80;
        y[48] = 23;
        x[49] = 68;
        y[49] = 85;
        x[50] = 127;
        y[50] = 102;
        x[51] = 151;
        y[51] = 60;
        x[52] = 137;
        y[52] = 45;
        x[53] = 170;
        y[53] = 22;
        x[54] = 173;
        y[54] = 118;
        x[55] = 115;
        y[55] = 56;
        x[56] = 20;
        y[56] = 75;
        x[57] = 131;
        y[57] = 27;
        x[58] = 156;
        y[58] = 43;
        x[59] = 183;
        y[59] = 131;
*/

        Random rnd;

        for (int i = 0; i < cities_; i++) {
            x[i] = rnd.nextInt(5,scWidth_-5);
            y[i] = rnd.nextInt(5,scHeight_-5);

        }
    }
};
