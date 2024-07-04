#include "Window.hpp"

#include <iostream>

Window::Window(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    this->title = title;
    this->windowData.width = width;
    this->windowData.height = height;
    this->initialized = false;
    this->vsyncEnabled = enableVsync;
    this->Init();
}

Window::~Window()
{
    Shutdown();
}

void Window::onUpdate()
{
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

int Window::Init()
{
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    if (!glfwInit())
        return -1;

    this->window = glfwCreateWindow(this->windowData.width, this->windowData.height, this->title.c_str(), NULL, NULL);

    if (!window)
    {
        std::cout << "Fatal error. Failed to create window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (this->vsyncEnabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Fatal error. GLEW initialisation failed!\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, &(this->windowData));

    glfwSetWindowSizeCallback(this->window,
                              [](GLFWwindow *window, int width, int height)
                              {
                                  WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);
                                  data->width = width;
                                  data->height = height;

                                  ResizeWindowEvent event(width, height);
                                  data->eventFn(event);
                              });

    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *window, double x, double y)
                             {
                                 WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);

                                 MouseMovedEvent event(x, y);
                                 data->eventFn(event);
                             });

    glfwSetWindowCloseCallback(window,
                               [](GLFWwindow *window)
                               {
                                   WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);

                                   CloseWindowEvent event;
                                   data->eventFn(event);
                               });

    std::cout << "Successfully initialzed GLFW, GLEW and created window.\n";
    initialized = true;
    return 0;
}

void Window::Shutdown()
{
    initialized = false;
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Destroying window\n";
}