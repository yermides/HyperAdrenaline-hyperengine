#include "particle.hpp"

namespace hyper {

Particle::Particle(
    glm::vec3 const& position
,   glm::vec3 const& velocity
,   glm::vec3 const& color
,   float lifetime
,   float size
,   Type type
)   
:   m_position(position)
,   m_velocity(velocity)
,   m_color(color)
,   m_lifetime(lifetime)
,   m_size(size)
,   m_type(type)
{
}

}
