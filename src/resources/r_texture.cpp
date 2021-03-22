#include "r_texture.hpp"

namespace hyper {

RTexture::RTexture()
: Resource{}
{
}

RTexture::RTexture(std::string const& path)
: Resource{}
{
    this->loadFromFile(path);
}

RTexture::~RTexture()
{
    // Si se inicializó, destruye la textura
    if(m_textureImage)
    {
        SOIL_free_image_data(m_textureImage);
        m_textureImage = nullptr;
    }
    if(m_textureID)
        glDeleteTextures(1, &m_textureID);
}

void 
RTexture::initialize()
{
    // TODO:: revisar
    // a lo mejor esta linea sobra, comprueba si está inicializada
    if(m_textureID)     return;
    if(!m_textureImage) return;

    glGenTextures(1, &m_textureID);

    GLenum format { GL_RGBA }; // antes era GL_RGB por defecto
    // if (m_numComponents == 1)
    //     format = GL_RED;
    // else if (m_numComponents == 3)
    //     format = GL_RGB;
    // else if (m_numComponents == 4)
        // format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_textureImage);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    INFOLOG("RTexture inicializada.");
}

void 
RTexture::loadTexture(std::string const& path) 
{
    // Forzado a tratarla como 4 canales para el byte array
    m_textureImage = SOIL_load_image(
            path.c_str()
        , 	&m_width
        , 	&m_height
        , 	&m_numComponents
        ,   SOIL_LOAD_RGBA
        // , 	SOIL_LOAD_RGB
    );

    if(m_textureImage) {
        INFOLOG("Textura cargada desde el path " << VAR(path))
    } else {
        ERRLOG("Textura falló al cargar desde el path: " << VAR(path));
    }
}

void 
RTexture::loadFromFile(std::string const& path) 
{
    this->setName(path);
    this->loadTexture(path);
}

}