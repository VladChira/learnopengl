#pragma once

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Console.hpp"
#include "framebuffer/Framebuffer.hpp"
#include "Shader.hpp"

#include "ImGuizmo.h"

class OpenGlRenderer
{
public:
    OpenGlRenderer(float width, float height);
    void Render();
    unsigned int getFrameBufferTexture();
    void RescaleFrameBuffer(float width, float height);

    void enableGrid(bool enable) { this->drawGrid = enable; }

    bool drawGrid = true;
    bool drawLightGizmos = true;
    bool drawTransformGizmos = true;
    bool wireframeMode = false;

private:
    void initGrid();
    void initPointLightMarker();
    void initDirLightMarker();

    float width, height;
    FrameBuffer sceneBuffer;

    // Grid stuff

    Shader gridShader;
    unsigned int GRID_VAO;
    unsigned int GRID_LEN;

    // Point Light marker stuff
    Shader pointLightMarker;
    unsigned int pointLightMarker_VAO;

    // Directional Light marker stuff
    Shader dirLightMarker;
    unsigned int dirLightMarker_VAO;

    // Mesh shaders
    Shader meshShader;
};