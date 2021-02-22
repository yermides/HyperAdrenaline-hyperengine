#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "resource.hpp"
#include <string>

struct VertexData
{
    glm::vec3 position;
    //glm::vec3 normal;
    //glm::vec2 textureCoordinates;
    //glm::vec3 tangent;
    //glm::vec3 bitangent;
};

struct RMesh : public Resource
{
    explicit RMesh();
    ~RMesh();

    void draw();
    void loadMesh(const std::string& filepath);

    constexpr uint32_t getVertexCount() const noexcept 
        { return m_nVertex; }
private:
    GLuint vao;         // Vertex Array Object
    GLuint vbo_position;// Vertex Buffer Object (positions)
    GLuint vbo_color;   // Vertex Buffer Object (colors)

    uint32_t m_nVertex {0};
};
