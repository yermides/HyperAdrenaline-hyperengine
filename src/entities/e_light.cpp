#include "e_light.hpp"

ELight::ELight(/* args */)
:	Entity{}
// ,   m_intensity{0,0,0,0}
// ,   m_type{LightType::Directional}
// ,   m_aperture{0.0f}
// ,   m_aten_angular{0.0f}
// ,   m_aten_const{0.0f}
// ,   m_aten_linear{0.0f}
// ,   m_aten_quadratic{0.0f}
{
}

ELight::~ELight()
{
}

void 
ELight::draw(const glm::mat4& tranform)
{
    std::cout << "Soy una luz.\n";
}
