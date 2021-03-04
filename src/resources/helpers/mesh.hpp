#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <resources/helpers/texture.hpp>

struct Mesh 
{
    Mesh();
    ~Mesh();

    void initialize(void);
    void draw(void);            // Maybe pass a shader->programID

    // Size = vertex count * 3
    std::vector<GLfloat>    m_vertices          ;
    std::vector<GLfloat>    m_normals           ;

    // Size = vertex count * 2
    std::vector<GLfloat>    m_texture_coords    ;

    // Size = vertex count
    std::vector<GLuint>     m_indices           ;

    // Size = undetermined
    std::vector<Texture>    m_textures          ;
    
private:
    // Handlers
    GLuint vao, *vbo, ebo;      
};
