#include "Entity.h"


Entity::Entity(EntityType type, vec2 position) : type(type), position(position) {}

Entity::~Entity() {}

//Functions 

void Entity::destroy()
{
    destroyed = true;
}

bool Entity::isDestroyed()
{
    return destroyed;
}
