#include "e_light.hpp"

namespace hyen {

ELight::ELight()
:	Entity{}
{
    INFOLOG("Constructing ELight");
}

ELight::ELight(
    LightType type
,   LightIntensity intensity
,   LightAttenuation attenuation
,   LightAperture aperture
,   LightDirection direction
)
:	Entity{}
,   m_type(type)
,   m_intensity(intensity)
,   m_attenuation(attenuation)
,   m_aperture(aperture)
,   m_direction(direction)
{
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
