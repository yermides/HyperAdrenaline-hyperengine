#pragma once
#include <glm/glm.hpp>
#include <iostream> 	// TODO:: borrar después del debug

struct Entity
{
	explicit Entity() {};
	virtual ~Entity() {};  // posible, no sabemos aún
	
	virtual void draw(const glm::mat4& tranform) = 0;
};
