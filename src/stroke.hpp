#pragma once
#include <fea/rendering/color.hpp>

struct Stroke
{
    Stroke(fea::Color colour, int radius, int posX, int posY);

    fea::Color mColour;
    int mRadius;
    int mPosX;
    int mPosY;
};

