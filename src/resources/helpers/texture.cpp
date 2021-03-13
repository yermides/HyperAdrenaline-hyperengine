// #include <resources/helpers/texture.hpp>

// Texture::Texture()
// {
// }

// Texture::~Texture()
// {
// }

// GLuint 
// Texture::loadTexture(Cstring path, std::string const& directory, bool gamma)
// {
//     using namespace std;
//     string filename = string(path);
//     filename = directory + '/' + filename;

//     unsigned int textureID;
//     glGenTextures(1, &textureID);

//     int width, height, nrComponents;
//     // unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//     // auto data = SOIL_load_image("assets/plantilla.png", &width, &height, &nrComponents, SOIL_LOAD_RGB);
//     auto data = SOIL_load_image(
//         filename.c_str()
//         , &width
//         , &height
//         , &nrComponents
//         , SOIL_LOAD_RGB
//         // , SOIL_LOAD_RGBA
//     );
//     if (data)
//     {
//         GLenum format {GL_RGBA};
//         if (nrComponents == 1)
//             format = GL_RED;
//         else if (nrComponents == 3)
//             format = GL_RGB;
//         else if (nrComponents == 4)
//             format = GL_RGBA;

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         INFOLOG("Texture succesfully loaded at path: " << VAR(filename));
//     }
//     else
//     {
//         ERRLOG("Texture failed to load at path: " << filename);
//     }

//     SOIL_free_image_data(data);

//     m_id = textureID;
//     return textureID;
// }

