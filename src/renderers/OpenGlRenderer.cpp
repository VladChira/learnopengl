#include "OpenGlRenderer.hpp"

unsigned int OpenGlRenderer::getFrameBufferTexture()
{
    return sceneBuffer.getFrameTexture();
}

void OpenGlRenderer::RescaleFrameBuffer(float width, float height)
{
    sceneBuffer.RescaleFrameBuffer(width, height);
}

OpenGlRenderer::OpenGlRenderer(float width, float height)
{
    glEnable(GL_DEPTH_TEST);
    gridShader.init("../shaders/grid.vert", "../shaders/grid.frag");
    initGrid();

    meshShader.init("../shaders/shader.vert", "../shaders/shader.frag");

    this->width = width;
    this->height = height;
    sceneBuffer.Init(this->width, this->height);
}

void OpenGlRenderer::Render()
{
    std::shared_ptr<Camera> camera = SceneManager::GetInstance()->activeCamera;
    float *bgColor = SceneManager::GetInstance()->bgColor;

    sceneBuffer.Bind();
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), this->width / this->height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();

    gridShader.use();
    gridShader.setMat4("projection", projection);
    gridShader.setMat4("view", view);
    gridShader.setMat4("model", model);
    glBindVertexArray(GRID_VAO);
    glDrawElements(GL_LINES, GRID_LEN, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

    sceneBuffer.Unbind();
}

void OpenGlRenderer::initGrid()
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