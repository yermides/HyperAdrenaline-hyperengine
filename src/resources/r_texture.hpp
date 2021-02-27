#pragma once
#include <resources/resource.hpp>

// // TODO::
// struct Texture
// {
//     // Image imagen
// };

struct RTexture : public Resource
{
    RTexture(/* args */);
    ~RTexture();

    void loadTexture ( const std::string& path );
    void loadFromFile( const std::string& path ) final;
private:
    // Imagen imagenTextura
    Image m_textureImage;
};
