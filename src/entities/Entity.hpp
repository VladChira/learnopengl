#pragma once

#include <random>
#include <sstream>
#include <memory>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

enum class EntityType
{
    NullObject,
    Model,
    Mesh,
    Primitive,
    Light,
    Camera
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
        default:
            return "[Unknown Type]";
        }
    }

    float position[3];
    float rotation[3]; // Euler angles in degrees
    float scale[3];

    void resetTransform()
    {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;

        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;

        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
    }

    virtual glm::mat4 getModelMatrix()
    {
        glm::vec3 p = glm::vec3(position[0], position[1], position[2]);
        glm::vec3 r = glm::vec3(rotation[0], rotation[1], rotation[2]); // Euler angles in degrees
        glm::vec3 s = glm::vec3(scale[0], scale[1], scale[2]);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, p);

        model = glm::rotate(model, glm::radians(r.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(r.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(r.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, s);

        return model;
    }

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