#include "e_camera.hpp"

namespace hyen {

ECamera::ECamera(glm::mat4 const& projection)
: Entity{}, m_projection(projection)
{
}

ECamera::~ECamera()
{
}

void 
ECamera::draw(glm::mat4 const& transform) 
{
}

}
