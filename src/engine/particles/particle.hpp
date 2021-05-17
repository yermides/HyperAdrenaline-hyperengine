#pragma once
#include <glm/vec3.hpp>

namespace hyper {

// Usada en el system1, que como no va, pues equis dé
struct SParticle
{
    enum Type : int {
        Generator    = 0
    ,   Normal
    };

    explicit SParticle() = default;

    explicit SParticle(
        glm::vec3 const& position
    ,   glm::vec3 const& velocity
    ,   glm::vec3 const& color
    ,   float lifetime
    ,   float size
    ,   Type type
    );

    ~SParticle() = default;

    glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_color;
	float m_lifetime;
	float m_size;
    Type m_type;
};

}