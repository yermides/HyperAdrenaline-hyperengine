#pragma once
#include "entity.hpp"

struct ECamera : public Entity
{
	explicit ECamera();
	~ECamera();

	void draw(glm::mat4 const& tranform) final;

	constexpr void setPerspective(float x, float y, float z, float w) noexcept
		{ m_isPerspective = true; 	}	// TODO::

	constexpr void setParallel(float x, float y, float z, float w) noexcept
		{ m_isPerspective = false; 	}	// TODO::
		
private:
	bool m_isPerspective;
	float m_left, m_right, m_lower, m_upper, m_close, m_far;
};
