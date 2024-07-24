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
    glViewport(0, 0, renderer->getWidth(), renderer->getHeight());
    renderer->Render();
}

int Application::Start(std::string title, const unsigned int width, const unsigned int height, bool enableVsync)
{
    Logger::Init();
    LOG_INFO("Logging enabled. Welcome to my rasterizer!");

    stbi_set_flip_vertically_on_load(true);
    LOG_INFO("stbi flipping vertically on load");

    this->window = std::make_unique<Window>(title, width, height, enableVsync);

    eventDispatcher.addEventListener<MouseMovedEvent>(
        [](MouseMovedEvent &e)
        {
            // std::cout << "Mouse moved to " << e.getX() << " " << e.getY() << "\n";
        });
    LOG_INFO("Added event listener for mouse movement");

    eventDispatcher.addEventListener<ResizeWindowEvent>(
        [](ResizeWindowEvent &e)
        {
            // std::cout << "Window resized to " << e.getWidth() << " " << e.getHeight() << "\n";
        });
    LOG_INFO("Added event listener for window resize");

    eventDispatcher.addEventListener<CloseWindowEvent>(
        [&](CloseWindowEvent &e)
        {
            this->close = true;
            LOG_WARN("Close Window event triggered");
        });
    LOG_INFO("Added event listener for closing window");

    this->window->setEventCallback(
        [&](Event &event)
        {
            eventDispatcher.dispatch(event);
        });


    LOG_INFO("Creating OpenGL renderer instance");
    renderer = std::make_shared<OpenGlRenderer>(width, height);
    window->setRenderer(renderer);

    LOG_INFO("Starting main application loop");
    while (!this->shouldClose())
    {
        this->onUpdate();
        window->onUpdate();
    }
    LOG_WARN("Shutting down application...");
    window = nullptr; // let the unique_ptr free this
    return 0;
}