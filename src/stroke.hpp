#pragma once
#include <fea/rendering/color.hpp>

struct Stroke
{
    Stroke(int colour, int radius, int posX, int posY);

    fea::Color mColour;
    int mRadius;
    int mPosX;
    int mPosY;
};

