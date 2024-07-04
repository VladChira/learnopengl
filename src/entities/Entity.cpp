#include "Entity.hpp"

#include "Console.hpp"

Entity::Entity(EntityType type)
{
    UUID = generateUUID();
    this->name = UUID;
    this->type = type;

    Console::GetInstance()->addSuccesEntry("Created new entity with UUID " + UUID + " and of type " + TypeToString(type));

}