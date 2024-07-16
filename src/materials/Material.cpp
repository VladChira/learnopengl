#include "Material.hpp"

#include "Console.hpp"

Material::Material(MaterialType type) : Entity(EntityType::Material)
{
    this->setName("Material");
    this->materialType = type;
    Console::GetInstance()->addLogEntry("Created new material of type " + MaterialTypeToString(type));
}

Material::Material(MaterialType type, std::string name) : Entity(EntityType::Material)
{
    this->setName(name);
    this->materialType = type;
    Console::GetInstance()->addLogEntry("Created new material of type " + MaterialTypeToString(type));
}