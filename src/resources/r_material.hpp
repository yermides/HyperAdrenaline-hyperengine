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
    glm::vec3 m_Ka, m_Kd, m_Ks;     // coeficientes de reflexión ambiental, difusa, especular
    float m_Ns, d;                  // exponente especulary transparencia
    Image mapKa, mapKd, mapKs;      // Imagen mapaKa, mapaKd, mapaKs
    // ...
};
