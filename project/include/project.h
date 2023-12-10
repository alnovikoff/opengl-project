#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>

#include "defines.h"
#include "window.h"

class PROJECT_API Project
{
public:
    Project() {}
    void run();
    void process_input(double dt);

private:
    Window window;
};

#endif // PROJECT_H