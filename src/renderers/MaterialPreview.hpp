#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <queue>

#include <glad/glad.h>

#include "lights/PointLight.hpp"
#include "entities/SpherePrimitive.hpp"
#include "framebuffer/Framebuffer.hpp"
#include "materials/Material.hpp"
#include "cameras/Camera.hpp"

class MaterialPreview
{
public:

    static const int PREVIEW_SIZE = 256;
    static const int MAX_RENDERS_PER_FRAME = 50;

    void markForPreview(std::shared_ptr<Material> material);
    void renderPreviews();

    MaterialPreview();
    ~MaterialPreview();

    MaterialPreview(MaterialPreview &other) = delete;
    MaterialPreview(MaterialPreview &&) = delete;
    void operator=(const MaterialPreview &) = delete;

    static MaterialPreview *GetInstance();
    static void DestroyInstance();

    Shader previewShader;
    std::queue<std::shared_ptr<Material>> previewQueue;
    unsigned int fbo;
	unsigned int rbo;

private:
    static MaterialPreview *mp_instance;

    std::shared_ptr<PointLight> l1, l2, l3;
    std::shared_ptr<SpherePrimitive> sphere;
    std::shared_ptr<Camera> camera;
};