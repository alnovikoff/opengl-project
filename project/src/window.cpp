#include "window.h"

#include <iostream>

unsigned int Window::window_width = 800;
unsigned int Window::window_height = 600;

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    window_width = width;
    window_height = height;
}

Window::Window() : window(nullptr) {
    
}

bool Window::init()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif


    window = glfwCreateWindow(window_width, window_height, "Project", NULL, NULL);

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        //glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

void Window::set_parameters()
{
    glViewport(0 , 0, window_width, window_height);
    
    /// HERE IS AN ERROR
    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);

    //glfwSetKeyCallback(window, Keyboard::key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
    //glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    //glfwSetScrollCallback(window, Mouse::mouse_wheel_callback);
}

void Window::update()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::new_frame()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::should_close()
{
    return glfwWindowShouldClose(window);
}

void Window::set_should_close(bool should_close)
{
    glfwSetWindowShouldClose(window, should_close);
}

Window::~Window()
{
    if (window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}