#pragma once
#include <resources/resource.hpp>
#include <glm/glm.hpp>

struct RMaterial : public Resource
{
    RMaterial(/* args */);
    ~RMaterial();

    void loadMaterial( Cstring path );
    void loadFromFile( const std::string& path ) final;

private:
    // coeficientes de reflexión ambiental, difusa, especular
    glm::vec3 m_Ka, m_Kd, m_Ks;
    // exponente especulary transparencia
    float m_Ns, d;

    // Imagen mapaKa, mapaKd, mapaKs
    // ...
};
