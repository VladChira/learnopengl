#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "ImGuizmo.h"

#include "Shader.hpp"
#include "entities/Entity.hpp"

enum class LightType
{
    PointLight,
    DirectionalLight,
    SpotLight
};

class Light : public Entity
{
public:
    Light(LightType type);
    Light(LightType type, std::string name);
    // virtual ~Light();

    LightType getType() { return type; }

    static inline std::string LightTypeToString(LightType lt)
    {
        switch (lt)
        {
        case LightType::PointLight:
            return "Point Light";
        case LightType::DirectionalLight:
            return "Directional Light";
        case LightType::SpotLight:
            return "Spot Light";
        default:
            return "[Unknown Type]";
        }
    }

    virtual void setUniforms(Shader &shader, int indexOfLight) = 0;

private:
    std::string name;
    LightType type;
    unsigned int ID = 0;
};