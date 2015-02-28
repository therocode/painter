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

    // copy the result texture to a new, canvas texture for drawing on
    mCanvasTexture.create(result.getSize().x, result.getSize().y, result.getPixelData(), false, true);

    int32_t currentScore = calculateScore(result, original);

    std::cout << "Starting score is " << currentScore << "\n";

    for(int i = 0; i < n; ++i)
    {
        fea::Color strokeColour(mColourRange(gen), mColourRange(gen), mColourRange(gen));
        Stroke stroke(strokeColour, mRadiusRange(gen), mPosXRange(gen), mPosYRange(gen));

        // change to make h and w actually the pixel position //
        for(int h = 0; h < (stroke.mRadius * 2); ++h)
        {
            for(int w = 0; w < (stroke.mRadius * 2); ++w)
            {
                float deltaX = abs(stroke.mPosX - w);
                float deltaY = abs(stroke.mPosY - h);

                float distanceFromCentre = sqrt((deltaX * deltaX) + (deltaY * deltaY));
                float distanceAsDecimal = distanceFromCentre / float(stroke.mRadius);
                int transparency = int(lerp(0.0f, 255.0f, distanceAsDecimal));

                // unnecessary when h and w pixel pos, do up there then //
                int xalle = std::max(std::min(stroke.mPosX - stroke.mRadius + w, int(mCanvasTexture.getSize().x) - 1), 0);
                int yalle = std::max(std::min(stroke.mPosY - stroke.mRadius + h, int(mCanvasTexture.getSize().y) - 1), 0);

                fea::Color pixelColour = stroke.mColour;
                //pixelColour.setA(transparency);
                mCanvasTexture.setPixel(xalle, yalle, pixelColour);
            }
        }
    }

    mCanvasTexture.update();
    result.create(result.getSize().x, result.getSize().y, mCanvasTexture.getPixelData(), false, true);
}

float Painter::lerp(float a, float b, float amount)
{
    amount = std::max(std::min(amount, 1.0f), 0.0f);
    float difference = b - a;
    float position = difference * amount;
    return (a + position);
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
