#pragma once

#include <string>
#include <functional>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ImGuizmo.h"

#include "Events.hpp"
#include "renderers/OpenGlRenderer.hpp"

class Window
{
public:
    using EventCallbackFn = std::function<void(Event &)>;

    Window(std::string title, const unsigned int width, const unsigned int height, bool vsync);
    ~Window();

    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;

    void onUpdate();
    void setEventCallback(const EventCallbackFn &callback)
    {
        this->windowData.eventFn = callback;
    }

    unsigned int getWidth() const { return this->windowData.width; }
    unsigned int getHeight() const { return this->windowData.height; };

    float clearColor[4] = {1.0f, 0.0f, 0.0f, 0.0f};

private:
    struct WindowData
    {
        unsigned int width;
        unsigned int height;
        EventCallbackFn eventFn;
    };

    int Init();
    void Shutdown();

    GLFWwindow *window;

    ImGuiIO *io;

    std::string title;
    WindowData windowData;
    bool initialized = false;
    bool vsyncEnabled = true;

    unsigned int addLightButtonTex;

    std::unique_ptr<OpenGlRenderer> renderer;

    bool TextureFromFile(std::string filename, unsigned int &textureID);
};