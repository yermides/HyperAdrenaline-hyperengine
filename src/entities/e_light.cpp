#include "e_light.hpp"

namespace hyper {

ELight::ELight()
:	Entity{}
{
    INFOLOG("Constructing ELight");
}

ELight::~ELight()
{
    INFOLOG("Deleting ELight");
}

void 
ELight::draw(glm::mat4 const& tranform)
{
}

}
