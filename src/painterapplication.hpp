#pragma once
#include <fea/structure.hpp>

class PainterApplication : public fea::Application
{
    public:
        void setup(const std::vector<std::string>& args) override;
        void loop() override;
        void destroy() override;
};
