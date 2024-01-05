#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <filesystem>
#include <vector>

#include "defines.h"
#include "window.h"
#include "render/model.h"

class PROJECT_API Project
{
public:
    Project() {}

    void initialize();
    void run_render();
    void run_sound();
    void process_input(double dt);

    std::filesystem::path get_asset_path();

private:
    Window window;
};

#endif // PROJECT_H