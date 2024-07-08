#include "Entity.hpp"

#include "Console.hpp"

Entity::Entity(EntityType type)
{
    UUID = generateUUID();
    this->name = UUID;
    this->type = type;

    Console::GetInstance()->addLogEntry("Created new entity of type " + TypeToString(type) +  "  [" + UUID + "]");
}

Entity::Entity(EntityType type, std::string name)
{
    UUID = generateUUID();
    this->name = name;
    this->type = type;

    Console::GetInstance()->addLogEntry("Created new entity of type " + TypeToString(type) +  "  [" + UUID + "]");
}