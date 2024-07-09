#include "Light.hpp"

#include "Console.hpp"

Light::Light(LightType type) : Entity(EntityType::Light)
{
    this->name = "Light";
    Console::GetInstance()->addLogEntry("Created new light of type " + LightTypeToString(type));
}

Light::Light(LightType type, std::string name) : Entity(EntityType::Light)
{
    this->name = name;
    Console::GetInstance()->addLogEntry("Created new light of type " + LightTypeToString(type));
}