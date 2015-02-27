#pragma once
#include <fea/rendering/texture.hpp>

class Painter
{
    public:
        Painter();
        void paint(const fea::Texture& original, fea::Texture& result, int n);
    
    private:
};
