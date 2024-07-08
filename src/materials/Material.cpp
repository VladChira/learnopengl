#include "Material.hpp"

#include "Console.hpp"

Material::Material(MaterialType type)
{
    this->name = "Material";
    Console::GetInstance()->addLogEntry("Created new material of type " + MaterialTypeToString(type));
}

Material::Material(MaterialType type, std::string name)
{
    this->name = name;
    Console::GetInstance()->addLogEntry("Created new material of type " + MaterialTypeToString(type));
}