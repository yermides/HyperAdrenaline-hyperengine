#pragma once
#include <array>
#include <resources/resource.hpp>
#include <resources/r_texture.hpp>
#include <managers/resource_manager.hpp>

using SkyboxNamelist = std::array<std::string, 6>;

#define default_skybox_texture_name_top     "_top"
#define default_skybox_texture_name_bottom  "_bottom"
#define default_skybox_texture_name_left    "_left"
#define default_skybox_texture_name_right   "_right"
#define default_skybox_texture_name_front   "_front"
#define default_skybox_texture_name_back    "_back"

namespace hyper {

struct RSkybox : public Resource
{
    explicit RSkybox();
    explicit RSkybox(std::string const& path);
    ~RSkybox();

    void loadSkybox(SkyboxNamelist const& paths);
    void loadFromFile(std::string const& path) final;

private:
    // Texturas para cada cara de la skybox
    RTexture 
        *m_top      { nullptr }
    ,   *m_bottom   { nullptr }
    ,   *m_left     { nullptr }
    ,   *m_right    { nullptr }
    ,   *m_front    { nullptr }
    ,   *m_back     { nullptr }
    ;

    inline static float const m_positions[] {
        // Posiciones de los vértices, iguales para toda skybox         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Identificador de la textura cargada a partir de las 6 texturas
    ProgramIdentifier m_textureID { 0 };      
};

}