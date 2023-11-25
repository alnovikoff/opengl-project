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

private:
    Window window;
};

#endif // PROJECT_H