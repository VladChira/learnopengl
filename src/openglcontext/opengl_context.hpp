#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../stb_image.h"
#include "framebuffer.h"
#include "../shader.hpp"
#include "../cameras/camera.h"

class OpenGLContext
{
private:
    float width, height;

public:
    Camera camera;
    FrameBuffer sceneBuffer;
    float clear_color[3];
    float dirColor[3], pointColor[3], spotColor[3];
    float dirPos[3], pointPos[3];
    float phongConstants[4];

    OpenGLContext(float w, float h);

    void init();

    void render();

    unsigned int getFrameBufferTexture();
};
