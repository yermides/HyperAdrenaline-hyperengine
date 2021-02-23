#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "resource.hpp"
#include <string>
#include <vector>

struct Mesh 
{
    std::vector<float>  m_vertices          ;
    std::vector<float>  m_normals           ;
    std::vector<float>  m_texture_coords    ;
    std::vector<int>    m_indexes           ;
    // std::vector<Texture> m_texture ;

    void draw() 
    {
        // TODO:: seguir por https://youtu.be/vvuRKSyqQnY?t=409
    }
};

// struct VertexData
// {
//     glm::vec3 position;
//     glm::vec3 normal;
//     glm::vec2 textureCoordinates;
//     glm::vec3 tangent;
//     glm::vec3 bitangent;
// };

struct RMesh : public Resource
{
    explicit RMesh();
    ~RMesh();

    void draw();
    void draw_ejemploCubo();    // TODO:: borrarla en cuanto se pueda

    void loadMesh(const std::string& filepath);
    void loadMesh_ejemploCubo(const std::string& filepath); // TODO:: borrarla en cuanto se pueda

    void loadFromFile( const std::string& path ) final;

    constexpr uint32_t getVertexCount() const noexcept 
        { return m_nVertex; }
private:
    // Utils para el ejemplo de antes de cargar las mallas, aún podrían servir
    GLuint vao;         // Vertex Array Object
    GLuint vbo_position;// Vertex Buffer Object (positions)
    GLuint vbo_color;   // Vertex Buffer Object (colors)
    uint32_t m_nVertex {0};

    // Con las mallas
    std::vector<Mesh*> m_meshes;
    // vertices, normals, texturecoords (or std::vector<VertexData>)
};
