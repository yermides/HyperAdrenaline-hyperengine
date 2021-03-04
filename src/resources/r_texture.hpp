#pragma once
#include <resources/resource.hpp>
#include <resources/helpers/texture.hpp>

struct RTexture : public Resource
{
    explicit RTexture(void);
    explicit RTexture(std::string const& path);
    ~RTexture(void);

    void loadTexture (std::string const& path);
    void loadFromFile(std::string const& path) final;
private:
    Image m_textureImage;
};
