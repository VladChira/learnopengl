#include "MaterialPreview.hpp"

#include "gui/Console.hpp"

MaterialPreview *MaterialPreview::mp_instance{nullptr};

MaterialPreview::MaterialPreview()
{
    previewShader.init("../shaders/shader.vert", "../shaders/shader.frag");

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, PREVIEW_SIZE, PREVIEW_SIZE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    camera = std::make_shared<Camera>(glm::vec3(3.85f, 1.01f, 3.39f), glm::vec3(0.0f, 1.0f, 0.0f), -136.9f, -12.6f);

    glm::mat4 tr = glm::mat4(1.0f);
    l1 = std::make_shared<PointLight>();
    tr = glm::translate(tr, glm::vec3(3.65f, 3.71f, -2.73f));
    l1->transform = tr;
    l1->intensity = 0.4f;

    tr = glm::mat4(1.0f);
    l2 = std::make_shared<PointLight>();
    tr = glm::translate(tr, glm::vec3(0.66f, 1.22f, 4.51f));
    l2->transform = tr;
    l2->intensity = 1.0f;

    tr = glm::mat4(1.0f);
    l3 = std::make_shared<PointLight>();
    tr = glm::translate(tr, glm::vec3(-0.68, 2.74f, 0.60f));
    l3->transform = tr;
    l3->intensity = 0.2f;

    sphere = std::make_shared<SpherePrimitive>(1.0f, 36, 36, true);
}

void MaterialPreview::markForPreview(std::shared_ptr<Material> material)
{
    // First, check if the material preview texture was generated, and do that if not
    if (material->previewTexture == 0)
    {
        glGenTextures(1, &(material->previewTexture));
        glBindTexture(GL_TEXTURE_2D, material->previewTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PREVIEW_SIZE, PREVIEW_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    previewQueue.push(material);
}

void MaterialPreview::renderPreviews()
{
    int renderCount = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    while (!previewQueue.empty() && renderCount < MAX_RENDERS_PER_FRAME)
    {
        glViewport(0, 0, PREVIEW_SIZE, PREVIEW_SIZE);
        std::shared_ptr<Material> material = previewQueue.front();
        previewQueue.pop();

        Console::GetInstance()->addSuccesEntry("Rendered preview for material " + material->getName());

        // Bind the correct texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, material->previewTexture, 0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render here
        previewShader.use();
        previewShader.setInt("numPointLights", 3);
        previewShader.setInt("numDirLights", 0);
        previewShader.setInt("numSpotLights", 0);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = camera->GetProjectionMatrix(PREVIEW_SIZE, PREVIEW_SIZE);
        glm::mat4 view = camera->GetViewMatrix();

        previewShader.setMat4("projection", projection);
        previewShader.setMat4("view", view);
        previewShader.setMat4("model", model);

        l1->setUniforms(previewShader, 0);
        l2->setUniforms(previewShader, 1);
        l3->setUniforms(previewShader, 2);

        sphere->material = std::dynamic_pointer_cast<PhongMaterial>(material);
        sphere->Draw(previewShader);

        renderCount++;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MaterialPreview::~MaterialPreview()
{
}

MaterialPreview *MaterialPreview::GetInstance()
{
    if (mp_instance == nullptr)
        mp_instance = new MaterialPreview();
    return mp_instance;
}

void MaterialPreview::DestroyInstance()
{
    delete mp_instance;
    mp_instance = nullptr;
}