#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Entity.hpp"

#include "../materials/PhongMaterial.hpp"

class Primitive : public Entity
{
public:
    Primitive() : Entity(EntityType::Primitive)
    {
        material = std::make_shared<PhongMaterial>();
        resetTransform();
    }

    Primitive(std::string name) : Primitive()
    {
        this->setName(name);
    }

    ~Primitive()
    {
    }

    virtual void Draw(Shader &shader) = 0;

    std::shared_ptr<PhongMaterial> material;
};