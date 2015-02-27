#include "painterapplication.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "texturemaker.hpp"

PainterApplication::PainterApplication() :
    mWindow(new fea::SDL2WindowBackend()),
    mInputHandler(new fea::SDL2InputBackend()),
    mRenderer(fea::Viewport({800, 600}, {0, 0}, fea::Camera({800.0f / 2.0f, 600.0f / 2.0f}))),
    mDisplayQuad({800, 600})
{
}

void PainterApplication::setup(const std::vector<std::string>& args)
{
    mWindow.create(fea::VideoMode(800, 600), "painter");
    mRenderer.setup();

    mOriginal = makeTexture("image.png");

    mDisplayQuad.setTexture(mOriginal);
    mDisplayQuad.setPosition({-800.0f / 2.0f, -600.0f / 2.0f});
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
    mPainter.paint(mOriginal, mResult, 1000);   

    //output
    mRenderer.queue(mDisplayQuad);
    mRenderer.render();
    mWindow.swapBuffers();
}

void PainterApplication::destroy()
{
    mWindow.close();
}
