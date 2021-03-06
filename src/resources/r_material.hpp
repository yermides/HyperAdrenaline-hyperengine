#pragma once
#include <glm/glm.hpp>
#include <resources/resource.hpp>
#include <util/macros.hpp>

struct RMaterial : public Resource
{
    explicit RMaterial();
    explicit RMaterial(std::string const& path);
    ~RMaterial();

    void loadMaterial(std::string const& path);
    void loadFromFile(std::string const& path) final;

private:
    glm::vec3 m_Ka, m_Kd, m_Ks;     // coeficientes de reflexión ambiental, difusa, especular
    float m_Ns, d;                  // exponente especulary transparencia
    Image mapKa, mapKd, mapKs;      // Imagen mapaKa, mapaKd, mapaKs
    // ...
};
