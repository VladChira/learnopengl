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

    glm::mat4 getModelMatrix() override
    {
        glm::vec3 p = glm::vec3(position[0], position[1], position[2]);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, p);
        return model;
    }

    virtual void setUniforms(Shader &shader, int indexOfLight) override
    {
        std::string uniformName = "pointLights[" + std::to_string(indexOfLight) + "]";
        shader.setVec3((uniformName + ".position").c_str(), glm::vec3(position[0], position[1], position[2]));
        shader.setVec3((uniformName + ".color").c_str(), lightColor[0] * intensity, lightColor[1] * intensity, lightColor[2] * intensity);
        shader.setFloat((uniformName + ".constant").c_str(), constantTerm);
        shader.setFloat((uniformName + ".linear").c_str(), linearTerm);
        shader.setFloat((uniformName + ".quadratic").c_str(), quadraticTerm);
    }

    float lightColor[3];
    float intensity = 1.0f;
    float constantTerm;
    float linearTerm;
    float quadraticTerm;
};