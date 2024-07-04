#pragma once

#include "Window.hpp"

#include "Events.hpp"
#include "EventDispatcher.hpp"

#include "stb_image.h"

#include <memory>

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
    std::unique_ptr<Window> window;
    EventDispatcher eventDispatcher;
    bool close = false;
};