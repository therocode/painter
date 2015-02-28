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
        int32_t calculateScore(const fea::Texture& source, const fea::Texture& target) const;
        std::random_device rd;
        std::mt19937 gen;
        float lerp(float a, float b, float amount);

        fea::Texture mCanvasTexture;
};
