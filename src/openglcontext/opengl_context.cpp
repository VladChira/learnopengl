#include "opengl_context.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "../shader.hpp"
#include "framebuffer.h"

Shader cubeShader, lightShader;
unsigned int VBO, VAO;
unsigned int lightVAO;
unsigned int texture;

unsigned int OpenGLContext::getFrameBufferTexture()
{
    return sceneBuffer.getFrameTexture();
}

void OpenGLContext::init()
{
    glEnable(GL_DEPTH_TEST);
    cubeShader.init("../shaders/shader.vert", "../shaders/shader.frag");
    lightShader.init("../shaders/light.vert", "../shaders/light.frag");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int imageWidth, imageHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("../textures/container.jpg", &imageWidth, &imageHeight, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    sceneBuffer.Init(this->width, this->height);
}

void OpenGLContext::render()
{
    sceneBuffer.Bind();
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubeShader.use();
    cubeShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
    cubeShader.setVec3("lightPos", lightPos[0], lightPos[1], lightPos[2]);
    cubeShader.setVec3("viewPos", camera.Position);
    cubeShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    cubeShader.setFloat("material.ka", phongConstants[0]);
    cubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    cubeShader.setFloat("material.kd", phongConstants[1]);
    cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cubeShader.setFloat("material.ks", phongConstants[2]);
    cubeShader.setFloat("material.shininess", phongConstants[3]);
    cubeShader.setInt("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), this->width / this->height, 0.1f, 100.0f);
    cubeShader.setMat4("projection", projection);
    glm::mat4 view = camera.GetViewMatrix();
    cubeShader.setMat4("view", view);

    // render one cube
    glm::mat4 model = glm::mat4(1.0f);
    cubeShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // render another
    cubeShader.setVec3("material.ambient", lightColor[0], lightColor[1], lightColor[2]);
    cubeShader.setVec3("material.diffuse", lightColor[0], lightColor[1], lightColor[2]);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.1f, 1.8f));
    cubeShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    lightShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader.setMat4("model", model);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    sceneBuffer.Unbind();
}

OpenGLContext::OpenGLContext(float w, float h) : camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
    this->width = w;
    this->height = h;
    clear_color[0] = 0.0f;
    clear_color[1] = 0.0f;
    clear_color[2] = 0.0f;

    lightColor[0] = 1.0f;
    lightColor[1] = 1.0f;
    lightColor[2] = 1.0f;

    lightPos[0] = 1.2f;
    lightPos[1] = 1.0f;
    lightPos[2] = 2.0f;

    phongConstants[0] = 0.1f;
    phongConstants[1] = 0.8f;
    phongConstants[2] = 0.6f;
    phongConstants[3] = 32.0f;
}