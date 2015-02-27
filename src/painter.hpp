#pragma once
#include <fea/rendering/texture.hpp>
#include <random>
#include "stroke.hpp"

class Painter
{
    public:
        Painter();
        void paint(const fea::Texture& original, fea::Texture& result, int n);
    
    private:
        std::random_device rd;
        std::mt19937 gen;

        fea::Texture mCanvasTexture;
};
