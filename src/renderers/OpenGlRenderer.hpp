#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Console.hpp"
#include "../framebuffer/Framebuffer.hpp"
#include "../Shader.hpp"

class OpenGlRenderer
{
public:
    OpenGlRenderer(float width, float height);
    void Render();
    unsigned int getFrameBufferTexture();
    void RescaleFrameBuffer(float width, float height);

    void enableGrid(bool enable) { this->drawGrid = enable; }

private:
    void initGrid();

    float width, height;
    FrameBuffer sceneBuffer;

    // Grid stuff
    bool drawGrid = true;
    Shader gridShader;
    unsigned int GRID_VAO;
    unsigned int GRID_LEN;

    // Mesh shaders
    Shader meshShader;
};