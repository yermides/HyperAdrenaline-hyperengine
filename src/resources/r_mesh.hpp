#pragma once
#include <resources/resource.hpp>
#include <resources/helpers/mesh.hpp>
#include <util/macros.hpp>

struct RMesh : public Resource
{
    explicit RMesh();
    ~RMesh();

    void draw();

    void loadMesh(const std::string& filepath);

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
    // Buffer handles, arrays
    // GLuint m_vaoHandles;
    // GLuint* m_vboHandles;

    // vertices, normals, texturecoords (or std::vector<VertexData>)
};

// struct VertexData
// {
//     glm::vec3 position;
//     glm::vec3 normal;
//     glm::vec2 textureCoordinates;
//     glm::vec3 tangent;
//     glm::vec3 bitangent;
// };
