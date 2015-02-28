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
    std::cout << "Preparing to do " << n << " iterations! :)\n";
    std::uniform_int_distribution<> mColourRange(0, 255);
    //std::uniform_int_distribution<> mRadiusRange(5, 200);
    std::normal_distribution<> mRadiusRange(100, 30);
    std::uniform_int_distribution<> mPosXRange(0, original.getSize().x);    // possibly extend these outside of the canvas border a bit
    std::uniform_int_distribution<> mPosYRange(0, original.getSize().y);
    int32_t pixelAmount = original.getSize().x * original.getSize().y;

    // copy the result texture to a new, canvas texture for drawing on
    mCanvasTexture.create(result.getSize().x, result.getSize().y, fea::Color::Black, false, true);
    uint8_t* sourcePixels = result.getPixelData();
    uint8_t* targetPixels = mCanvasTexture.getPixelData();
    std::copy(sourcePixels, sourcePixels + pixelAmount * 4, targetPixels);

    for(int32_t i = 0; i < n; ++i)
    {
        fea::Color strokeColour(mColourRange(gen), mColourRange(gen), mColourRange(gen));
        Stroke stroke(strokeColour, mRadiusRange(gen), mPosXRange(gen), mPosYRange(gen));

        int32_t wStart = std::max(stroke.mPosX - stroke.mRadius, 0);
        int32_t hStart = std::max(stroke.mPosY - stroke.mRadius, 0);
        int32_t wLimit = std::min(stroke.mPosX + stroke.mRadius, int32_t(mCanvasTexture.getSize().x) - 1);
        int32_t hLimit = std::min(stroke.mPosY + stroke.mRadius, int32_t(mCanvasTexture.getSize().y) - 1);

        int32_t scoreDelta = 0;

        for(int32_t h = hStart; h < hLimit; ++h)
        {
            for(int32_t w = wStart; w < wLimit; ++w)
            {
                targetPixelColour = original.getPixel(w, h);
                oldPixelColour = mCanvasTexture.getPixel(w, h);
                float deltaX = abs(stroke.mPosX - w);
                float deltaY = abs(stroke.mPosY - h);

                float distanceFromCentre = std::hypot(deltaX, deltaY);
                float distanceAsDecimal = distanceFromCentre / float(stroke.mRadius);
                lerpColour(stroke.mColour, oldPixelColour, distanceAsDecimal, newPixelColour);
                
                mCanvasTexture.setPixel(w, h, newPixelColour);

                scoreDelta += calculateScore(newPixelColour, targetPixelColour) - calculateScore(oldPixelColour, targetPixelColour);
            }
        }

        if(scoreDelta < 0)
        {//score was better, keep the image
            std::cout << "improved score by " << -scoreDelta << "\n";
            uint8_t* sourcePixels = mCanvasTexture.getPixelData();
            uint8_t* targetPixels = result.getPixelData();
            std::copy(sourcePixels, sourcePixels + pixelAmount * 4, targetPixels);
        }
        else
        {//discard changes
            uint8_t* sourcePixels = result.getPixelData();
            uint8_t* targetPixels = mCanvasTexture.getPixelData();
            std::copy(sourcePixels, sourcePixels + pixelAmount * 4, targetPixels);
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
