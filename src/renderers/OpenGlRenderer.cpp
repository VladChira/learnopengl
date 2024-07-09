#include "OpenGlRenderer.hpp"

#include "../SceneManager.hpp"

#include "../entities/CubePrimitive.hpp"
#include "../lights/PointLight.hpp"

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

    pointLightMarker.init("../shaders/pointlightmarker.vert", "../shaders/pointlightmarker.frag");
    initPointLightMarker();

    meshShader.init("../shaders/shader.vert", "../shaders/shader.frag");

    this->width = width;
    this->height = height;
    sceneBuffer.Init(this->width, this->height);

    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->setName("Backpack");
    model->Init("../models/backpack/scene.gltf");
    SceneManager::GetInstance()->addModel(model);

    std::shared_ptr<CubePrimitive> cube = std::make_shared<CubePrimitive>(1.0f, 1.0f, 1.0f);
    cube->setName("Test Cube");
    SceneManager::GetInstance()->addPrimitive(cube);

    std::shared_ptr<PointLight> light = std::make_shared<PointLight>();
    light->setName("Default Point Light");
    SceneManager::GetInstance()->addLight(light);
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

    if (drawLightGizmos)
    {
        pointLightMarker.use();
        model = glm::translate(model, glm::vec3(0.2f, 0.0f, 2.5f));
        pointLightMarker.setMat4("projection", projection);
        pointLightMarker.setMat4("view", view);

        for (int i = 0; i < SceneManager::GetInstance()->lights.size(); i++)
        {
            auto light = SceneManager::GetInstance()->lights[i];
            pointLightMarker.setMat4("model", light->getModelMatrix());

            if (light->getType() == LightType::PointLight)
            {
                PointLight *pLight = dynamic_cast<PointLight *>(light.get());
                pointLightMarker.setVec3("markerColor", pLight->lightColor[0], pLight->lightColor[1], pLight->lightColor[2]);
            }
            else
            {
                pointLightMarker.setVec3("markerColor", 1.0, 1.0, 1.0);
            }

            glBindVertexArray(pointLightMarker_VAO);
            glLineWidth(2.0f);
            glDrawArrays(GL_LINES, 0, 24);
            glBindVertexArray(0);
            model = glm::mat4(1.0f);
        }
    }

    if (drawGrid)
    {
        gridShader.use();
        gridShader.setMat4("projection", projection);
        gridShader.setMat4("view", view);
        gridShader.setMat4("model", model);
        glBindVertexArray(GRID_VAO);
        glDrawElements(GL_LINES, GRID_LEN, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }

    meshShader.use();

    // meshShader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
    // meshShader.setVec3("dirLight.color", 1.0f, 1.0f, 1.0f);
    meshShader.setInt("numPointLights", SceneManager::GetInstance()->nrOfPointLights);
    meshShader.setInt("numDirLights", SceneManager::GetInstance()->nrOfPointLights);
    meshShader.setInt("numSpotLights", SceneManager::GetInstance()->nrOfSpotLights);
    for (int i = 0; i < SceneManager::GetInstance()->lights.size(); i++)
    {
        auto light = SceneManager::GetInstance()->lights[i];
        light->setUniforms(meshShader, i);
    }

    meshShader.setMat4("projection", projection);
    meshShader.setMat4("view", view);

    for (int i = 0; i < SceneManager::GetInstance()->models.size(); i++)
    {
        auto model = SceneManager::GetInstance()->models[i];
        meshShader.setMat4("model", model->getModelMatrix());
        model->Draw(meshShader);
    }

    for (int i = 0; i < SceneManager::GetInstance()->primitives.size(); i++)
    {
        auto primitive = SceneManager::GetInstance()->primitives[i];
        meshShader.setMat4("model", primitive->getModelMatrix());
        primitive->Draw(meshShader);
    }

    sceneBuffer.Unbind();
}

void OpenGlRenderer::initPointLightMarker()
{
    GLfloat markerVertices[] = {
        // X-axis lines
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.0f,
        0.0f,
        // Y-axis lines
        0.0f,
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.0f,
        // Z-axis lines
        0.0f,
        0.0f,
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        // Additional lines
        0.5f,
        0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        0.5f,
        0.0f,
        0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        0.0f,
        0.5f,
        0.5f,
        0.0f,
        -0.5f,
        -0.5f,
    };
    unsigned int VBO;

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &pointLightMarker_VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(pointLightMarker_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(markerVertices), markerVertices, GL_STATIC_DRAW);

    // Specify vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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