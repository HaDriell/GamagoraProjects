#include "Entity.h"


Entity::Entity(EntityType type) : type(type) {}

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
