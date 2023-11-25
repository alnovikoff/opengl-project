#ifndef WINDOW_H
#define WINDOW_H

#include <other/glad/glad.h>
#include <GLFW/glfw3.h>

#include "defines.h"

class PROJECT_API Window
{
public:
    static unsigned int window_width;
    static unsigned int window_height;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    Window();
    ~Window();

    bool init();
    void set_parameters();

    void update();
    void new_frame();

    bool should_close();
    void set_should_close(bool should_close);

    GLFWwindow* get_window() {
        return window;
    }

private:
    GLFWwindow* window;
};

#endif // WINDOW_H
