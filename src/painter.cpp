#include "painter.hpp"
#include "stroke.hpp"
#include <iostream>

Painter::Painter()  :
    gen(rd())
{
}

void Painter::paint(const fea::Texture& original, fea::Texture& result, int n)
{
    std::cout << "Preparing to do " << n << " iterations! :)\n";
    std::uniform_int_distribution<> mColourRange(0, 255);
    std::uniform_int_distribution<> mRadiusRange(5, 20);
    std::uniform_int_distribution<> mPosXRange(0, original.getSize().x);
    std::uniform_int_distribution<> mPosYRange(0, original.getSize().y);

    int32_t currentScore = calculateScore(result, original);

    std::cout << "Starting score is " << currentScore << "\n";

    for(int i = 0; i < n; ++i)
    {
        Stroke stroke(mColourRange(gen), mRadiusRange(gen), mPosXRange(gen), mPosYRange(gen));

        for(int h = 0; h < (stroke.mRadius * 2); ++h)
        {
            for(int w = 0; w < (stroke.mRadius * 2); ++w)
            {
            }
        }
    }
}

int32_t Painter::calculateScore(const fea::Texture& source, const fea::Texture& target) const
{
    int32_t pixelAmount = source.getSize().x * source.getSize().y;
    fea::Color* sourcePixels = (fea::Color*) source.getPixelData();
    fea::Color* targetPixels = (fea::Color*) target.getPixelData();

    int32_t score = 0;

    std::cout << "pixel amount is " << pixelAmount << "\n";
    for(int32_t i = 0; i < pixelAmount; ++i)
    {
        score += abs((int32_t)sourcePixels[i].r() - (int32_t)targetPixels[i].r()) +
                 abs((int32_t)sourcePixels[i].g() - (int32_t)targetPixels[i].g()) +
                 abs((int32_t)sourcePixels[i].b() - (int32_t)targetPixels[i].b());
    }

    return score;
}
