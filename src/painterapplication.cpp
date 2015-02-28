#include "painterapplication.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "texturemaker.hpp"

PainterApplication::PainterApplication() :
    mWindow(new fea::SDL2WindowBackend()),
    mInputHandler(new fea::SDL2InputBackend()),
    mDisplayQuad({800, 600})
{
    mWindow.create(fea::VideoMode(800, 600, 32), "painter");
    mWindow.setFramerateLimit(60);
}

void PainterApplication::setup(const std::vector<std::string>& args)
{
    mRenderer = std::unique_ptr<fea::Renderer2D>(new fea::Renderer2D(fea::Viewport({800, 600}, {0, 0}, fea::Camera({800.0f / 2.0f, 600.0f / 2.0f}))));
    mRenderer->setup();

    mOriginal = makeTexture("image.png");
    auto size = mOriginal.getSize();
    mResult.create(size.x, size.y, fea::Color::Black, false, true);

    mDisplayQuad.setTexture(mResult);
}

void PainterApplication::loop()
{
    //input
    fea::Event event;

    while(mInputHandler.pollEvent(event))
    {   
        if(event.type == fea::Event::KEYPRESSED)
        {
            if(event.key.code == fea::Keyboard::ESCAPE)
                quit();
        }
    }

    //process
    mPainter.paint(mOriginal, mResult, 100);   

    //output
    mRenderer->clear();
    mRenderer->queue(mDisplayQuad);
    mRenderer->render();
    mWindow.swapBuffers();
}

void PainterApplication::destroy()
{
    mWindow.close();
}
