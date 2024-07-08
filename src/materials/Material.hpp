#pragma once

#include <string>
#include <memory>
#include <iostream>

enum class MaterialType
{
    Phong
};

class Material
{
public:
    Material(MaterialType type);
    Material(MaterialType type, std::string name);
    // virtual ~Material();

    void setName(std::string newName) { this->name = newName; }
    std::string getName() { return this->name; };

    MaterialType getType() { return type; }

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

private:
    std::string name;
    MaterialType type;
    unsigned int ID = 0;
};