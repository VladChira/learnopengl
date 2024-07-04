#pragma once

#include <random>
#include <sstream>
#include <memory>

enum class EntityType
{
    NullObject,
    Mesh,
    Light,
    Camera
};

class Entity
{
public:
    Entity(EntityType type);
    // virtual ~Entity();

    void setName(std::string newName) { this->name = newName; }
    std::string getName() { return this->name; };

    std::vector<std::shared_ptr<Entity>> getChildren() { return children; }

    EntityType getType() { return type; }

    std::string getUUID() { return UUID; }

    static inline std::string TypeToString(EntityType et)
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
        default:
            return "[Unknown Type]";
        }
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