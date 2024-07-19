#pragma once

#include <glad/glad.h>

#include "Entity.hpp"

#include "materials/PhongMaterial.hpp"

enum class PrimitiveType
{
    Cube,
    Sphere
};

class Primitive : public Entity
{
public:
    Primitive(PrimitiveType type) : Entity(EntityType::Primitive)
    {
        this->primitiveType = type;
        material = std::make_shared<PhongMaterial>();
        resetTransform();
    }

    Primitive(PrimitiveType type, std::string name) : Primitive(type)
    {
        this->setName(name);
    }

    ~Primitive()
    {
    }

    virtual void Draw(Shader &shader) = 0;

    PrimitiveType primitiveType;
    std::shared_ptr<PhongMaterial> material;
};