#pragma once
#include <fea/structure.hpp>
#include <fea/userinterface.hpp>

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
};
