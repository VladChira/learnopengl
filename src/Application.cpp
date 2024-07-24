#include "Application.hpp"
#include "Console.hpp"

#include <iostream>

#include "Logger.hpp"

Application::Application()
{
    
}

Application::~Application()
{
    
}

void Application::onUpdate()
{
}

int Application::Start(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    Logger::Init();
    Logger::GetLogger()->info("Logging enabled");

    stbi_set_flip_vertically_on_load(true);
    Logger::GetLogger()->info("stbi flipping vertically on load");

    this->window = std::make_unique<Window>(title, width, height, enableVsync);

    eventDispatcher.addEventListener<MouseMovedEvent>(
        [](MouseMovedEvent& e)
        {
            // std::cout << "Mouse moved to " << e.getX() << " " << e.getY() << "\n";
        }
    );
    Logger::GetLogger()->info("Added event listener for mouse movement");

    eventDispatcher.addEventListener<ResizeWindowEvent>(
        [](ResizeWindowEvent& e)
        {
            // std::cout << "Window resized to " << e.getWidth() << " " << e.getHeight() << "\n";
        }
    );
    Logger::GetLogger()->info("Added event listener for window resize");

    eventDispatcher.addEventListener<CloseWindowEvent>(
        [&](CloseWindowEvent& e)
        {
            this->close = true;
        }
    );
    Logger::GetLogger()->info("Added event listener for closing window");

    this->window->setEventCallback(
        [&](Event &event)
        {
            eventDispatcher.dispatch(event);
        });


    Logger::GetLogger()->info("Starting main application loop");
    while (!this->shouldClose())
    {
        window->onUpdate();
        this->onUpdate();
    }
    window = nullptr; // let the unique_ptr free this
    return 0;
}