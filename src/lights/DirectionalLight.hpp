#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight() : Light(LightType::DirectionalLight)
    {
        lightColor[0] = 1.0f;
        lightColor[1] = 1.0f;
        lightColor[2] = 1.0f;

        direction[0] = 0.0f;
        direction[1] = 1.0f;
        direction[2] = 0.0f;
    }

    glm::mat4 &getTransform() override
    {
        // float position[3], rotation[3], scale[3];
        // ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), position, rotation, scale);
        // rotation[0] = 0.0f;
        // rotation[1] = 0.0f;
        // rotation[2] = 0.0f;
        // scale[0] = 1.0f;
        // scale[1] = 1.0f;
        // scale[2] = 1.0f;

        // ImGuizmo::RecomposeMatrixFromComponents(position, rotation, scale, (float *)glm::value_ptr(getTransform()));
        return transform;
    }

    virtual void setUniforms(Shader &shader, int indexOfLight) override
    {
        std::string uniformName = "dirLights[" + std::to_string(indexOfLight) + "]";
        shader.setVec3((uniformName + ".direction").c_str(), glm::normalize(glm::vec3(-direction[0], -direction[1], -direction[2])));
        shader.setVec3((uniformName + ".color").c_str(), lightColor[0] * intensity, lightColor[1] * intensity, lightColor[2] * intensity);
    }

    float lightColor[3];
    float direction[3];
    float intensity = 1.0f;
};