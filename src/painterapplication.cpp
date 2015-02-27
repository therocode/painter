#include "painterapplication.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>

PainterApplication::PainterApplication() :
    mWindow(new fea::SDL2WindowBackend()),
    mInputHandler(new fea::SDL2InputBackend())
{
}

void PainterApplication::setup(const std::vector<std::string>& args)
{
    mWindow.create(fea::VideoMode(800, 600), "painter");
}

void PainterApplication::loop()
{
    fea::Event event;

    while(mInputHandler.pollEvent(event))
    {   
        if(event.type == fea::Event::KEYPRESSED)
        {
            if(event.key.code == fea::Keyboard::ESCAPE)
                quit();
        }
    }

    mWindow.swapBuffers();
}

void PainterApplication::destroy()
{
    mWindow.close();
}
