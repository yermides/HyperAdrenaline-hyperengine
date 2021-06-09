#include <resources/helpers/mesh.hpp>

namespace hyen {

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    m_vertices.clear();
    m_normals.clear();
    m_tangents.clear();
    m_bitangents.clear();

    m_texture_coords.clear();
    m_indices.clear();

    for(auto m : m_materials)
    {
        delete m;
        m = nullptr;
    }

    m_materials.clear();

    // Limpiando buffers
    if(vbo) 
    {
        glDeleteBuffers(3, vbo);
        delete[] vbo;
    }

    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void 
Mesh::initialize(void)
{
    // se necesitan más vbo's por cómo hace él la struct, que le pasa los datos en bruto seguidos y luego los diferencia
    // nosotros necesitamos un buffer por cada tipo de datos por no estar seguidos en memoria, de ahí el array de vbo

    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    vbo = new GLuint[3];
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices.front(), GL_STATIC_DRAW);  

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat), &m_normals.front(), GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, m_texture_coords.size() * sizeof(GLfloat), &m_texture_coords.front(), GL_STATIC_DRAW); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices.front(), GL_STATIC_DRAW);

    // set the vertex attribute pointers

    // vertex Positions
    glEnableVertexAttribArray(0);	
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // unbing vertex array, we've already sent it the data
    glBindVertexArray(0);

    // Initialize materials
    for(auto material : m_materials)
        material->initialize();
}

// Para cada buffer 
// {
//      Asociar los datos en memoria al buffer   (glBufferData)
//      Enlazar el buffer con su attribute       (glBindBuffer)
//      indicar estructura del buffer            (glVertexAttribPointer)
//      Habilitar el buffer                      (glEnableVertexAttribArray)
// }
// Dibujar                                      (glDrawElements)

void 
Mesh::draw(RShader* const shader)
{
    for(auto m : m_materials)
        m->draw(shader);

    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // always good practice to set everything back to defaults once configured.
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

}