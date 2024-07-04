#include "Application.hpp"

#include <iostream>

Application::Application()
{
    std::cout << "Starting application\n";
}

Application::~Application()
{
    std::cout << "Stopping application\n";
}

void Application::onUpdate()
{
}

int Application::Start(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    this->window = std::make_unique<Window>(title, width, height, enableVsync);

    eventDispatcher.addEventListener<MouseMovedEvent>(
        [](MouseMovedEvent& e)
        {
            // std::cout << "Mouse moved to " << e.getX() << " " << e.getY() << "\n";
        }
    );

    eventDispatcher.addEventListener<ResizeWindowEvent>(
        [](ResizeWindowEvent& e)
        {
            // std::cout << "Window resized to " << e.getWidth() << " " << e.getHeight() << "\n";
        }
    );

    eventDispatcher.addEventListener<CloseWindowEvent>(
        [&](CloseWindowEvent& e)
        {
            this->close = true;
        }
    );

    this->window->setEventCallback(
        [&](Event &event)
        {
            eventDispatcher.dispatch(event);
        });

    while (!this->shouldClose())
    {
        window->onUpdate();
        this->onUpdate();
    }
    window = nullptr; // let the unique_ptr free this
    return 0;
}