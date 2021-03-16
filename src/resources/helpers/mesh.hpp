#pragma once
#include <string>
#include <vector>
#include <util/typealiases.hpp>
#include <util/macros.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// #include <resources/helpers/texture.hpp>
#include <resources/r_texture.hpp>
#include <resources/r_material.hpp>

namespace hyper {

struct Mesh 
{
    explicit Mesh();
    ~Mesh();

    void initialize(void);
    void draw(ProgramIdentifier const shaderID = 0);

    // Size = vertex count * 3
    std::vector<GLfloat>    m_vertices          ;
    std::vector<GLfloat>    m_normals           ;

    // Size = vertex count * 2
    std::vector<GLfloat>    m_texture_coords    ;

    // Size = vertex count
    std::vector<GLuint>     m_indices           ;

    // Size = undetermined
    std::vector<RTexture*>  m_textures          ;

    // Size = undetermined
    std::vector<RMaterial*>  m_materials        ;
    
private:
    // Handlers
    GLuint vao, *vbo, ebo;      
};

}