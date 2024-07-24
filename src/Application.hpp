#pragma once

#include <memory>

#include "stb_image.h"

#include "Window.hpp"

#include "Events.hpp"
#include "EventDispatcher.hpp"
#include "renderers/OpenGlRenderer.hpp"

class Application
{
public:
    Application();
    virtual ~Application();

    Application(const Application &) = delete;
    Application(Application &&) = delete;
    Application &operator=(const Application &) = delete;
    Application &operator=(Application &&) = delete;

    virtual int Start(std::string title, const unsigned int width, const unsigned int height, bool enableVsync);
    virtual void onUpdate();

    bool shouldClose() { return close; }

private:
    EventDispatcher eventDispatcher;

    std::unique_ptr<Window> window;
    bool close = false;

    std::shared_ptr<OpenGlRenderer> renderer;

};