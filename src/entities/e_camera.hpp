#pragma once
#include <entities/entity.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hyper {

struct ECamera : public Entity
{
	// explicit ECamera();
	explicit ECamera(glm::mat4 const& projection = glm::perspective (
		glm::radians(45.0f) // 45° Field of View
		, 16.0f / 9.0f      // 16:9 ratio
		, 0.1f              // display range : 0.1 unit <--
		, 100.0f            // --> 100 units
	));
	~ECamera();

	void draw(glm::mat4 const& transform) final;

	constexpr void setPerspective(float x, float y, float z, float w) noexcept
		{ m_isPerspective = true; 	}	// TODO::

	constexpr void setParallel(float x, float y, float z, float w) noexcept
		{ m_isPerspective = false; 	}	// TODO::
		
	constexpr glm::mat4 const& getViewMatrix() const noexcept
		{ return m_view; 				}

	constexpr glm::mat4 const& getProjectionMatrix() const noexcept
		{ return m_projection; 			}
	
private:
	float m_left, m_right, m_lower, m_upper, m_close, m_far;
	bool m_isPerspective;

	// Matriz perspectiva por defecto
	glm::mat4 m_projection;
	glm::mat4 m_view;
};

}