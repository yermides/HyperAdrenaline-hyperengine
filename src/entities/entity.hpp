#pragma once
#include <glm/glm.hpp>
#include <iostream> 	// TODO:: borrar después del debug
#include <util/typealiases.hpp>

struct Entity
{
	explicit Entity() {};
	virtual ~Entity() {};  // posible, no sabemos aún
	
	virtual void draw(const glm::mat4& tranform) = 0;

	constexpr ProgramIdentifier getProgramID() const noexcept
		{ return m_programID; }
	constexpr void 				setProgramID(ProgramIdentifier newID) noexcept
		{ m_programID = newID; }
protected:
	ProgramIdentifier m_programID;
};
