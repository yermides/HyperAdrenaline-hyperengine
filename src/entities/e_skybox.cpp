#include "e_skybox.hpp"

namespace hyen {
    
ESkybox::ESkybox() 
: Entity{}
{
}

ESkybox::ESkybox(SkyboxNamelist const& paths)
{
    loadFromFile(paths);
}

ESkybox::~ESkybox()
{
    if(m_textureID)
        glDeleteTextures(1, &m_textureID);

    m_top     = nullptr;
    m_bottom  = nullptr;
    m_left    = nullptr;
    m_right   = nullptr;
    m_front   = nullptr;
    m_back    = nullptr;
}

void 
ESkybox::loadFromFile(SkyboxNamelist const& paths)
{
    // Check de si ya ha sido inicializado
    if(m_textureID) return;

    // Carga
    m_top     = ResourceManager::getResource_t<RTexture>( paths.at(0) );
    m_bottom  = ResourceManager::getResource_t<RTexture>( paths.at(1) );
    m_left    = ResourceManager::getResource_t<RTexture>( paths.at(2) );
    m_right   = ResourceManager::getResource_t<RTexture>( paths.at(3) );
    m_front   = ResourceManager::getResource_t<RTexture>( paths.at(4) );
    m_back    = ResourceManager::getResource_t<RTexture>( paths.at(5) );

    // Inicialización de la textura de 6 caras
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

    // Inicialización de los atributos de los vértices
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions), &m_positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    INFOLOG("ESkybox inicializada.");
}

void
ESkybox::draw(glm::mat4 const& tranform)
{
    // if(!m_shader) return;

    // m_shader->bind();
    // m_shader->setInt("skybox", 0);

    // INFOLOG("Hola desde ESkybox");

    // Pasarle los valores al shader después de inicializarse
}

}
