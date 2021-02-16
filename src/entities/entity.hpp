#pragma once
#include <glm/glm.hpp>

struct Entity
{
    virtual ~Entity();  // posible, no sabemos aún
    virtual void draw(const glm::mat4& tranform) = 0;
};
