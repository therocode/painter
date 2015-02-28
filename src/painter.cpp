#include "painter.hpp"
#include "stroke.hpp"

Painter::Painter()  :
    gen(rd())
{
}

void Painter::paint(const fea::Texture& original, fea::Texture& result, int n)
{
    std::uniform_int_distribution<> mColourRange(0, 255);
    std::uniform_int_distribution<> mRadiusRange(5, 20);
    std::uniform_int_distribution<> mPosXRange(0, original.getSize().x);
    std::uniform_int_distribution<> mPosYRange(0, original.getSize().y);

    // copy the result texture to a new, canvas texture for drawing on
    mCanvasTexture.create(result.getSize().x, result.getSize().y, result.getPixelData(), false, true);

    for(int i = 0; i < n; ++i)
    {
        Stroke stroke(mColourRange(gen), mRadiusRange(gen), mPosXRange(gen), mPosYRange(gen));

        for(int h = 0; h < (stroke.mRadius * 2); ++h)
        {
            for(int w = 0; w < (stroke.mRadius * 2); ++w)
            {
                float deltaX = abs(stroke.mPosX - w);
                float deltaY = abs(stroke.mPosY - h);

                float distanceFromCentre = sqrt((deltaX * deltaX) + (deltaY * deltaY));
                float distanceAsDecimal = distanceFromCentre / float(stroke.mRadius);
                int transparency = int(lerp(0.0f, 255.0f, distanceAsDecimal));

                fea::Color pixelColour = stroke.mColour;
                pixelColour.setA(transparency);
                mCanvasTexture.setPixel(stroke.mPosX + w, stroke.mPosY + h, pixelColour);
            }
        }
    }
}

float Painter::lerp(float a, float b, float amount)
{
    amount = std::max(std::min(amount, 1.0f), 0.0f);
    float difference = b - a;
    float position = difference * amount;
    return (a + position);
}
