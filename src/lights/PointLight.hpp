#pragma once

#include "Light.hpp"

class PointLight : public Light
{
public:
    PointLight() : Light(LightType::PointLight)
    {
        lightColor[0] = 1.0f;
        lightColor[1] = 1.0f;
        lightColor[2] = 1.0f;

        constantTerm = 1.0f;
        linearTerm = 0.09f;
        quadraticTerm = 0.032f;
    }

    glm::mat4 &getTransform() override
    {
        return transform;
    }

    virtual void setUniforms(Shader &shader, int indexOfLight) override
    {
        float position[3], rotation[3], scale[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), position, rotation, scale);

        std::string uniformName = "pointLights[" + std::to_string(indexOfLight) + "]";
        shader.setVec3((uniformName + ".position").c_str(), glm::vec3(position[0], position[1], position[2]));
        shader.setVec3((uniformName + ".color").c_str(), lightColor[0] * intensity, lightColor[1] * intensity, lightColor[2] * intensity);
        shader.setFloat((uniformName + ".constant").c_str(), constantTerm);
        shader.setFloat((uniformName + ".linear").c_str(), linearTerm);
        shader.setFloat((uniformName + ".quadratic").c_str(), quadraticTerm);

        if (!enabled)
        {
            shader.setVec3((uniformName + ".color").c_str(), 0, 0, 0);
        }
    }

    float lightColor[3];
    float intensity = 1.0f;
    float constantTerm;
    float linearTerm;
    float quadraticTerm;
};