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
