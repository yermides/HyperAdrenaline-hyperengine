#include "r_skybox.hpp"

namespace hyper {

RSkybox::RSkybox()
{
}

RSkybox::RSkybox(std::string const& path)
{
    this->loadFromFile(path);
}

RSkybox::~RSkybox()
{
}

void
RSkybox::loadSkybox(SkyboxNamelist const& paths)
{
    // Check de si ya ha sido inicializado
    if(this->m_textureID) return;

    // Carga
    m_top     = ResourceManager::getResource_t<RTexture>( paths.at(0) );
    m_bottom  = ResourceManager::getResource_t<RTexture>( paths.at(1) );
    m_left    = ResourceManager::getResource_t<RTexture>( paths.at(2) );
    m_right   = ResourceManager::getResource_t<RTexture>( paths.at(3) );
    m_front   = ResourceManager::getResource_t<RTexture>( paths.at(4) );
    m_back    = ResourceManager::getResource_t<RTexture>( paths.at(5) );

    // Inicialización
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, m_right->getWidth(), m_right->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_right->getRawData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, m_left->getWidth(), m_left->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_left->getRawData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, m_top->getWidth(), m_top->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_top->getRawData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, m_bottom->getWidth(), m_bottom->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bottom->getRawData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, m_front->getWidth(), m_front->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_front->getRawData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, m_back->getWidth(), m_back->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_back->getRawData());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    INFOLOG("RSkybox inicializada.");
}

void
RSkybox::loadFromFile(std::string const& path)
{
    INFOLOG("Estoy cargando las texturas de la skybox");

    // Totalmente hardcoded, TODO:: parametrizar
    SkyboxNamelist namelist = {
            std::string("assets/skybox/top.jpg")
        ,   std::string("assets/skybox/bottom.jpg")
        ,   std::string("assets/skybox/left.jpg")
        ,   std::string("assets/skybox/right.jpg")
        ,   std::string("assets/skybox/front.jpg")
        ,   std::string("assets/skybox/back.jpg")
    };

    this->loadSkybox(namelist);
}

}
