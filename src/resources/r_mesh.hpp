#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <resources/resource.hpp>
#include <util/macros.hpp>

struct Mesh 
{
    std::vector<GLfloat>  m_vertices          ;
    std::vector<GLfloat>  m_normals           ;
    std::vector<GLfloat>  m_texture_coords    ;
    std::vector<GLint>    m_indexes           ;
    // std::vector<Texture> m_texture ;

    GLuint vao, *vbo;

    void draw() 
    {
        // TODO:: terminar y revisar

        // Para cada buffer {
        // Asociar los datos en memoria al buffer   (glBufferData)
        // Enlazar el buffer con su attribute       (glBindBuffer)
        // indicar estructura del buffer            (glVertexAttribPointer)
        // Habilitar el buffer                      (glEnableVertexAttribArray)
        // }
        // Dibujar                                  (glDrawElements)

        // LOG("[PASO 1] SETUP");

        // Hacer allocation del size del array de vbo's por cada valor "in" del shader. Más tarde cambiaré a std::vector
        auto vbos           = 4;
        auto vboallocsize   = vbos * sizeof(GLuint);
        vbo        = (GLuint*)malloc(vboallocsize);
        glGenBuffers(vbos, vbo);

        // Generar el primer vertex buffer object (vertex positions). Métodos bind y data
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_vertices.size() * sizeof(GLfloat),
            &m_vertices.front(),
            GL_STATIC_DRAW
        );

        // Generar el segundo vertex buffer object (vertex colors)
        // esto se quitará, son los mismos valores que las normales
        // solo para ver las caras más facilmente
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_normals.size() * sizeof(GLfloat),
            &m_normals.front(),
            GL_STATIC_DRAW
        );
        // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        // glBufferData(
        //     GL_ARRAY_BUFFER,
        //     m_vertices.size() * sizeof(GLfloat),
        //     &m_vertices.front(),
        //     GL_STATIC_DRAW
        // );

        // Generar el tercer vertex buffer object (vertex textcoords). Métodos bind y data
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_texture_coords.size() * sizeof(GLfloat),
            &m_texture_coords.front(),
            GL_STATIC_DRAW
        );

        // Generar el vertex array object que "contiene" a los vertex buffer objects anteriores
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Pequeño setup antes de dibujar elementos
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Generar el cuarto vertex buffer object (vertex normals). Element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
        glBufferData(
            GL_ARRAY_BUFFER,
            m_normals.size() * sizeof(GLfloat),
            &m_normals.front(),
            GL_STATIC_DRAW
        );

        // LOG("[PASO 2] DRAW");

        glBindVertexArray(vao);
    
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

        // m_indexes.size() = Número de caras
        // glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 3);

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

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
