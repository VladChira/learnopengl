#include "opengl_context.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "../shader.hpp"
#include "framebuffer.h"
#include "../model.hpp"

#include <vector>

Shader objectShader;

Shader gridShader;
unsigned int GRID_VAO;
unsigned int GRID_LEN;
Model model1, model2;

unsigned int OpenGLContext::getFrameBufferTexture()
{
    return sceneBuffer.getFrameTexture();
}

void initGrid();

void OpenGLContext::init()
{
    glEnable(GL_DEPTH_TEST);

    gridShader.init("../shaders/grid.vert", "../shaders/grid.frag");
    initGrid();

    stbi_set_flip_vertically_on_load(true);

    objectShader.init("../shaders/shader.vert", "../shaders/shader.frag");

    model1.Init("../models/adamHead/adamHead.gltf");
    model2.Init("../models/backpack/scene.gltf");

    sceneBuffer.Init(this->width, this->height);
}

void OpenGLContext::render()
{
    sceneBuffer.Bind();
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), this->width / this->height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    gridShader.use();
    gridShader.setMat4("projection", projection);
    gridShader.setMat4("view", view);
    gridShader.setMat4("model", model);
    glBindVertexArray(GRID_VAO);
    glDrawElements(GL_LINES, GRID_LEN, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    objectShader.use();
    objectShader.setMat4("projection", projection);
    objectShader.setMat4("view", view);

    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    objectShader.setMat4("model", model);
    model1.Draw(objectShader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
    objectShader.setMat4("model", model);
    model2.Draw(objectShader);

    sceneBuffer.Unbind();
}

OpenGLContext::OpenGLContext(float w, float h) : camera(glm::vec3(0.0f, 0.0f, 3.0f))
{

    this->width = w;
    this->height = h;
    clear_color[0] = 0.05f;
    clear_color[1] = 0.05f;
    clear_color[2] = 0.05f;

    dirColor[0] = 0.3f;
    dirColor[1] = 0.3f;
    dirColor[2] = 0.3f;

    pointColor[0] = 1.0f;
    pointColor[1] = 1.0f;
    pointColor[2] = 1.0f;

    spotColor[0] = 1.0f;
    spotColor[1] = 1.0f;
    spotColor[2] = 1.0f;

    dirPos[0] = 1.2f;
    dirPos[1] = 1.0f;
    dirPos[2] = 2.0f;

    pointPos[0] = 0.2f;
    pointPos[1] = 0.0f;
    pointPos[2] = 1.2f;

    phongConstants[0] = 0.1f;
    phongConstants[1] = 0.8f;
    phongConstants[2] = 0.6f;
    phongConstants[3] = 32.0f;
}

void initGrid()
{
    float gridSize = 40.0f;
    float yPos = 0.0f;
    int density = 30;

    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec4> indices;

    float halfSize = gridSize / 2.0f;
    float step = gridSize / density;

    // Generate vertices
    for (int j = 0; j <= density; ++j)
    {
        for (int i = 0; i <= density; ++i)
        {
            float x = -halfSize + i * step;
            float y = yPos;
            float z = -halfSize + j * step;
            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (int j = 0; j < density; ++j)
    {
        for (int i = 0; i < density; ++i)
        {
            int row1 = j * (density + 1);
            int row2 = (j + 1) * (density + 1);

            // Add line segments for grid lines
            indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
            indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
        }
    }

    glGenVertexArrays(1, &GRID_VAO);
    glBindVertexArray(GRID_VAO);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GRID_LEN = (GLuint)indices.size() * 4;
}