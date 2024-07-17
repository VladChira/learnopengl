#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "../entities/Entity.hpp"
#include "../Shader.hpp"

enum class MaterialType
{
    Phong
};

class Material : public Entity
{
public:
    Material(MaterialType type);
    Material(MaterialType type, std::string name);
    // virtual ~Material();

    MaterialType getMaterialType() { return materialType; }

    static inline std::string MaterialTypeToString(MaterialType mt)
    {
        switch (mt)
        {
        case MaterialType::Phong:
            return "Phong";
        default:
            return "[Unknown Type]";
        }
    }

    virtual void setUniforms(Shader &shader) = 0;

    unsigned int previewTexture = 0;

private:
    MaterialType materialType;
};