#pragma once
#include <fea/render2d.hpp>
#include <fea/structure.hpp>
#include <fea/userinterface.hpp>
#include "painter.hpp"

class PainterApplication : public fea::Application
{
    public:
        PainterApplication();
        void setup(const std::vector<std::string>& args) override;
        void loop() override;
        void destroy() override;
    private:
        fea::Window mWindow;
        fea::InputHandler mInputHandler;

        fea::Texture mOriginal;
        fea::Texture mResult;
        Painter mPainter;

        fea::Quad mDisplayQuad;
        fea::Renderer2D mRenderer;
};
