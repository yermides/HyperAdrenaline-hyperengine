#pragma once
#include <glm/glm.hpp>
#include <resources/resource.hpp>
#include <resources/r_texture.hpp>
#include <managers/resource_manager.hpp>
#include <assimp/material.h>
#include <util/macros.hpp>

struct RMaterial : public Resource
{
    explicit RMaterial();
    explicit RMaterial(std::string const& path);
    ~RMaterial();

    void loadMaterial(std::string const& path);
    void loadMaterial(const aiMaterial* const amaterial);
    void loadFromFile(std::string const& path) final;

    constexpr glm::vec3 const& getColorAmbient(void)        const noexcept
        { return m_Ka; }
    constexpr glm::vec3 const& getColorDiffuse(void)        const noexcept
        { return m_Kd; }
    constexpr glm::vec3 const& getColorSpecular(void)       const noexcept
        { return m_Ks; }
    constexpr float const      getShinyness(void)           const noexcept
        { return m_Ns; }
    constexpr float const      getTransparency(void)        const noexcept
        { return m_d;  }

private:
    glm::vec3 m_Ka, m_Kd, m_Ks;                 // coeficientes de reflexión ambiental, difusa, especular
    float m_Ns, m_d;                            // exponente especular y transparencia
    RTexture *m_mapKa, *m_mapKd, *m_mapKs;      // Imagen mapaKa, mapaKd, mapaKs
};

// Image m_mapKa, m_mapKd, m_mapKs;    // Imagen mapaKa, mapaKd, mapaKs
// Más propiedades...
