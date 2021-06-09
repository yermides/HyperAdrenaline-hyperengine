#pragma once
#include <glm/glm.hpp>
#include <managers/resource_manager.hpp>
#include <resources/r_texture.hpp>
#include <resources/r_shader.hpp>
#include <assimp/material.h>
#include <util/macros.hpp>
#include <util/functions.hpp>

#define default_material_ambient_color      {1.0f,1.0f,1.0f}
#define default_material_diffuse_color      {0.8f,0.8f,0.8f}
#define default_material_specular_color     {0.5f,0.5f,0.5f}
#define default_material_specular_exponent  1.45f
#define default_material_opacity            1.0f

namespace hyen {

struct RMaterial : public Resource
{
    explicit RMaterial();
    explicit RMaterial(std::string const& path);
    explicit RMaterial(aiMaterial* amaterial, std::string const& search_directory = "assets");
    ~RMaterial();

    void initialize(void);
    void draw(RShader* const shader);

    void loadMaterial(std::string const& path);
    void loadMaterial(aiMaterial* amaterial, std::string const& search_directory = "assets");
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
    RTexture 
            *m_mapKa { nullptr }
        ,   *m_mapKd { nullptr }
        ,   *m_mapKs { nullptr }
        ,   *m_mapKn { nullptr };      // Imagen mapaKa, mapaKd, mapaKs, mapaKn
};

}

// Image m_mapKa, m_mapKd, m_mapKs;    // Imagen mapaKa, mapaKd, mapaKs
// Más propiedades...
