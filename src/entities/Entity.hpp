#pragma once

#include <random>
#include <sstream>
#include <memory>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class EntityType
{
    NullObject,
    Model,
    Mesh,
    Primitive,
    Light,
    Camera,
    Material
};

class Entity
{
public:
    Entity(EntityType type);
    Entity(EntityType type, std::string name);
    virtual ~Entity();

    void setName(std::string newName) { this->name = newName; }
    std::string getName() { return this->name; };

    std::vector<std::shared_ptr<Entity>> &getChildren() { return children; }

    EntityType getType() { return type; }

    std::string getUUID() { return UUID; }

    static inline std::string EntityTypeToString(EntityType et)
    {
        switch (et)
        {
        case EntityType::NullObject:
            return "NullObject";
        case EntityType::Mesh:
            return "Mesh";
        case EntityType::Light:
            return "Light";
        case EntityType::Camera:
            return "Camera";
        case EntityType::Model:
            return "Model";
        case EntityType::Primitive:
            return "Primitive";
        case EntityType::Material:
            return "Material";
        default:
            return "[Unknown Type]";
        }
    }

    glm::mat4 transform;
    void resetTransform()
    {
        transform = glm::mat4(1.0f);
    }

    virtual glm::mat4 &getTransform()
    {
        return transform;
    }

    bool enabled = true;

private:
    std::string name;
    std::string UUID;
    EntityType type;

    std::vector<std::shared_ptr<Entity>> children;

    std::string generateUUID()
    {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        std::uniform_int_distribution<int> dist(0, 15);

        const char *v = "0123456789abcdef";
        const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

        std::string res;
        for (int i = 0; i < 16; i++)
        {
            if (dash[i])
                res += "-";
            res += v[dist(rng)];
            res += v[dist(rng)];
        }
        return res;
    }
};