#pragma once
#include <unordered_map>
#include <string>
#include <stdint.h>
#include <util/typealiases.hpp>
#include <util/macros.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

struct Texture
{
    Texture(/* args */);
    ~Texture();

    // Análogo al [TextureFromFile] de learnOpenGL
    GLuint loadTexture(Cstring path, std::string const& directory, bool gamma = false);

    uint32_t m_id;
    std::string m_type;
    std::string m_path;
};

// std::unordered_map<unsigned int, std::string> texturenames = {
//     { 1, ""}
// };
