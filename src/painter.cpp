#include "painter.hpp"
#include "stroke.hpp"
#include <iostream>
#include <cmath>

Painter::Painter()  :
    gen(rd())
{
}

void Painter::paint(const fea::Texture& original, fea::Texture& result, int32_t n)
{
    int32_t textureWidth = original.getSize().x;
    int32_t textureHeight = original.getSize().y;

    std::cout << "Preparing to do " << n << " iterations! :)\n";
    std::uniform_int_distribution<> mColourRange(0, 255);
    //std::uniform_int_distribution<> mRadiusRange(5, 200);
    std::normal_distribution<> mRadiusRange(100, 30);
    std::uniform_int_distribution<> mPosXRange(0, textureWidth);    // possibly extend these outside of the canvas border a bit
    std::uniform_int_distribution<> mPosYRange(0, textureHeight);
    int32_t pixelAmount = textureWidth * textureHeight;

    mCanvasTexture.create(textureWidth, textureHeight, fea::Color::Black, false, true);
    const uint8_t* originalPixels = original.getPixelData();
    uint8_t* resultPixels = result.getPixelData();
    uint8_t* canvasPixels = mCanvasTexture.getPixelData();
    // copy the result texture to a new, canvas texture for drawing on
    std::copy(resultPixels, resultPixels + pixelAmount * 4, canvasPixels);

    for(int32_t i = 0; i < n; ++i)
    {
        fea::Color strokeColour(mColourRange(gen), mColourRange(gen), mColourRange(gen));
        Stroke stroke(strokeColour, mRadiusRange(gen), mPosXRange(gen), mPosYRange(gen));

        int32_t wStart = std::max(stroke.mPosX - stroke.mRadius, 0);
        int32_t hStart = std::max(stroke.mPosY - stroke.mRadius, 0);
        int32_t wLimit = std::min(stroke.mPosX + stroke.mRadius, textureWidth - 1);
        int32_t hLimit = std::min(stroke.mPosY + stroke.mRadius, textureHeight - 1);

        int32_t scoreDelta = 0;

        int32_t pixelIndex = (hStart * textureWidth + wStart) * 4;

        float inverseStrokeRadius = 1.0f / stroke.mRadius;

        for(int32_t h = hStart; h < hLimit; ++h)
        {
            for(int32_t w = wStart; w < wLimit; ++w)
            {
                targetPixelColour.setR(originalPixels[pixelIndex]);
                targetPixelColour.setG(originalPixels[pixelIndex + 1]);
                targetPixelColour.setB(originalPixels[pixelIndex + 2]);

                oldPixelColour.setR(canvasPixels[pixelIndex]);
                oldPixelColour.setG(canvasPixels[pixelIndex + 1]);
                oldPixelColour.setB(canvasPixels[pixelIndex + 2]);

                float deltaX = stroke.mPosX - w;
                float deltaY = stroke.mPosY - h;

                float distanceFromCentre = std::hypot(deltaX, deltaY);
                float distanceAsDecimal = distanceFromCentre * inverseStrokeRadius;
                lerpColour(stroke.mColour, oldPixelColour, distanceAsDecimal, newPixelColour);
                
                canvasPixels[pixelIndex] = newPixelColour.r();
                canvasPixels[pixelIndex + 1] = newPixelColour.g();
                canvasPixels[pixelIndex + 2] = newPixelColour.b();

                scoreDelta += calculateScore(newPixelColour, targetPixelColour) - calculateScore(oldPixelColour, targetPixelColour);

                pixelIndex += 4;
            }
            pixelIndex += (textureWidth - (wLimit - wStart)) * 4;
        }

        if(scoreDelta < 0)
        {   //score was better, keep the image
            std::cout << "improved score by " << -scoreDelta << "\n";
            // copying from canvas to result
            std::copy(canvasPixels, canvasPixels + pixelAmount * 4, resultPixels);
        }
        else
        {   //discard changes
            // copying from result to canvas
            std::copy(resultPixels, resultPixels + pixelAmount * 4, canvasPixels);
        }
    }

    result.update();
}

void Painter::lerpColour(const fea::Color& a, const fea::Color& b, float amount, fea::Color& resultColour)
{
    amount = std::max(std::min(amount, 1.0f), 0.0f);
    float deltaR = int32_t(b.r()) - int32_t(a.r());
    float deltaG = int32_t(b.g()) - int32_t(a.g());
    float deltaB = int32_t(b.b()) - int32_t(a.b());

    uint8_t red   = a.r() + (deltaR * amount);
    uint8_t green = a.g() + (deltaG * amount);
    uint8_t blue  = a.b() + (deltaB * amount);

    resultColour.setR(red);
    resultColour.setG(green);
    resultColour.setB(blue);
}

int32_t Painter::calculateScore(const fea::Color& source, const fea::Color& target) const
{
    return abs((int32_t)source.r() - (int32_t)target.r()) +
           abs((int32_t)source.g() - (int32_t)target.g()) +
           abs((int32_t)source.b() - (int32_t)target.b());
}

//int32_t Painter::calculateScore(const fea::Texture& source, const fea::Texture& target) const
//{
//    int32_t pixelAmount = source.getSize().x * source.getSize().y;
//    fea::Color* sourcePixels = (fea::Color*) source.getPixelData();
//    fea::Color* targetPixels = (fea::Color*) target.getPixelData();
//
//    int32_t score = 0;
//
//    for(int32_t i = 0; i < pixelAmount; ++i)
//    {
//        score += abs((int32_t)sourcePixels[i].r() - (int32_t)targetPixels[i].r()) +
//                 abs((int32_t)sourcePixels[i].g() - (int32_t)targetPixels[i].g()) +
//                 abs((int32_t)sourcePixels[i].b() - (int32_t)targetPixels[i].b());
//    }
//
//    return score;
//}
