#pragma once
#include <array>
#include <entities/entity.hpp>
#include <resources/r_texture.hpp>
#include <managers/resource_manager.hpp>

using SkyboxNamelist = std::array<std::string, 6>;

namespace hyen {

struct ESkybox : public Entity
{
    explicit ESkybox();
    explicit ESkybox(SkyboxNamelist const& paths);
    ~ESkybox();

    void loadFromFile(SkyboxNamelist const& paths);
    void draw(glm::mat4 const& tranform) final;

// private:
    // Texturas para cada cara de la skybox
    RTexture 
        *m_top      { nullptr }
    ,   *m_bottom   { nullptr }
    ,   *m_left     { nullptr }
    ,   *m_right    { nullptr }
    ,   *m_front    { nullptr }
    ,   *m_back     { nullptr }
    ;

    // Posiciones de los vértices, iguales para toda skybox
    inline static float const m_positions[] {
        // Cara 1
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        // Cara 2
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        // Cara 3
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        // Cara 4
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
        // Cara 5
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        // Cara 6
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };

    // Identificador de la textura cargada a partir de las 6 texturas
    ProgramIdentifier m_textureID { 0 };

    GLuint m_vao, m_vbo;
};

}