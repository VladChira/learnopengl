#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../stb_image.h"
#include "framebuffer.h"
#include "../shader.hpp"
#include "camera.h"

class OpenGLContext
{
private:
    float width, height;

public:
    Camera camera;
    FrameBuffer sceneBuffer;
    float clear_color[3];
    float cubeColor[3];
    float lightPos[3];

    OpenGLContext(float w, float h);

    void init();

    void render();

    unsigned int getFrameBufferTexture();
};
