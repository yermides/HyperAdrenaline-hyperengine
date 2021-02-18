#pragma once
#include "entity.hpp"

struct ECamera : public Entity
{
	ECamera(/* args */);
	~ECamera();

	void draw(const glm::mat4& tranform) final;

	constexpr void setPerspective(const float& x, const float& y, const float& z, const float& w)	noexcept
		{ m_isPerspective = true; 	}	// TODO::

	constexpr void setParallel(const float& x, const float& y, const float& z, const float& w)	noexcept
		{ m_isPerspective = false; 	}	// TODO::
		
private:
	bool m_isPerspective;
	float m_left, m_right, m_lower, m_upper, m_close, m_far;
};
