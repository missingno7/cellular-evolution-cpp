#pragma once
//
// Created by MissingNO on 14.11.2019.
//

#include <cmath>

void LABtoRGB(int &r_r, int &g_r, int &b_r, int const l, int const a, int const b) {
    float r_t, g_t, b_t;

    float y = (l + 16.0) / 116.0;
    float x = a / 500.0 + y;
    float z = y - b / 200.0;

    x = 0.95047f * ((x * x * x > 0.008856f) ? x * x * x : (x - 16.0 / 116.0) / 7.787);
    y = 1.00000f * ((y * y * y > 0.008856f) ? y * y * y : (y - 16.0 / 116.0) / 7.787);
    z = 1.08883f * ((z * z * z > 0.008856f) ? z * z * z : (z - 16.0 / 116.0) / 7.787);

    r_t = x * 3.2406f + y * -1.5372f + z * -0.4986f;
    g_t = x * -0.9689f + y * 1.8758f + z * 0.0415f;
    b_t = x * 0.0557f + y * -0.2040f + z * 1.0570f;

    r_t = (r_t > 0.0031308f) ? (1.055f * std::pow(r_t, 1.0f / 2.4f) - 0.055f) : 12.92f * r_t;
    g_t = (g_t > 0.0031308f) ? (1.055f * std::pow(g_t, 1.0f / 2.4f) - 0.055f) : 12.92f * g_t;
    b_t = (b_t > 0.0031308f) ? (1.055f * std::pow(b_t, 1.0f / 2.4f) - 0.055f) : 12.92f * b_t;

    r_r = (int) (std::fmax(0, std::fmin(1, r_t)) * 255);
    g_r = (int) (std::fmax(0, std::fmin(1, g_t)) * 255);
    b_r = (int) (std::fmax(0, std::fmin(1, b_t)) * 255);
}