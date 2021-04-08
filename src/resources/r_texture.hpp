#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <resources/resource.hpp>
// #include <resources/helpers/texture.hpp>

namespace hyper {

struct RTexture : public Resource
{
    explicit RTexture(void);
    explicit RTexture(std::string const& path);
    ~RTexture();

    void initialize(void);
    // void bind(void);        // Podría ser útil un método así

    void loadTexture (std::string const& path);
    void loadFromFile(std::string const& path) final;

    constexpr ProgramIdentifier getProgramID(void)
        { return m_textureID;       }

    constexpr Image getRawData(void) const noexcept
        { return m_textureImage;    }

    constexpr int getWidth() const noexcept 
        { return m_width;           }

    constexpr int getHeight() const noexcept 
        { return m_height;           }

private:
    Image m_textureImage { nullptr };
    int 
        m_width         { 0 }
    ,   m_height        { 0 }
    ,   m_numComponents { 0 }
    ;
    ProgramIdentifier m_textureID { 0 };
};

}
