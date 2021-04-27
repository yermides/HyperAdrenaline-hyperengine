#pragma once
#include <entities/entity.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 45° Field of View, 16:9 ratio, display range : 0.1 unit <-- | --> 1000 units
#define default_camera_projection glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 1000.0f)

namespace hyper {

struct ECamera : public Entity
{
	// explicit ECamera();
	explicit ECamera(glm::mat4 const& projection = default_camera_projection);
	~ECamera();

	void draw(glm::mat4 const& transform) final;

	constexpr void setPerspective(float x, float y, float z, float w) noexcept
		{ m_isPerspective = true; 		}	// TODO::

	constexpr void setParallel(float x, float y, float z, float w) noexcept
		{ m_isPerspective = false; 		}	// TODO::
		
	constexpr glm::mat4 const& getViewMatrix(void) const noexcept
		{ return m_view; 				}

	constexpr void setViewMatrix(glm::mat4 const& view) noexcept
		{ m_view = view;				}

	constexpr glm::mat4 const& getProjectionMatrix(void) const noexcept
		{ return m_projection; 			}

	constexpr void setProjectionMatrix(glm::mat4 const& projection) noexcept
		{ m_projection = projection;	}
	
private:
	float m_left, m_right, m_lower, m_upper, m_close, m_far;
	bool m_isPerspective;

	// Matriz perspectiva por defecto
	glm::mat4 m_projection;
	glm::mat4 m_view;
};

}