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
        int32_t calculateScore(const fea::Color& source, const fea::Color& target) const;
        std::random_device rd;
        std::mt19937 gen;
        void lerpColour(const fea::Color& a, const fea::Color& b, float amount, fea::Color& resultColour);
        fea::Color targetPixelColour;
        fea::Color oldPixelColour;
        fea::Color newPixelColour;

        fea::Texture mCanvasTexture;
};
