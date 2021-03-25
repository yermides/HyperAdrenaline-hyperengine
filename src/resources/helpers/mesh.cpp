#include <resources/helpers/mesh.hpp>

namespace hyper {

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    m_vertices.clear();
    m_normals.clear();
    m_texture_coords.clear();
    m_indices.clear();
    m_textures.clear();

    for(auto m : m_materials)
        delete m;

    m_materials.clear();

    // free buffers
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    if(vbo) delete[] vbo;

    // textures are cleared once we exit thanks to the ResourceManager destructor
}

void 
Mesh::initialize(void)
{
    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    vbo = new GLuint[3];
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // load data into vertex buffers
    // se necesitan más vbo's por cómo hace él la struct, que le pasa los datos en bruto seguidos y luego los diferencia
    // nosotros necesitamos un buffer por cada tipo de datos por no estar seguidos en memoria, de ahí el array de vbo
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
    if(!shader) return;

    for(auto m : m_materials)
        m->draw(shader);

    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void 
Mesh::oldDraw(ProgramIdentifier const shaderID) 
{
    using namespace std;
    // INFOLOG("Mesh::draw longitud del array de texturas: " << VAR(m_textures.size()))


    // bind appropriate textures
    
    // unsigned int diffuseNr  = 1;
    // unsigned int specularNr = 1;
    // unsigned int normalNr   = 1;
    // unsigned int heightNr   = 1;

    for(uint32_t i {0}; i < m_textures.size(); ++i)
    {
        auto& texture = m_textures[i];

        // glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // // retrieve texture number (the N in diffuse_textureN)
        // string number;
        // string name = m_textures[i].type;
        // if(name == "texture_diffuse")
        //     number = std::to_string(diffuseNr++);
        // else if(name == "texture_specular")
        //     number = std::to_string(specularNr++); // transfer unsigned int to stream
        // else if(name == "texture_normal")
        //     number = std::to_string(normalNr++); // transfer unsigned int to stream
        //     else if(name == "texture_height")
        //     number = std::to_string(heightNr++); // transfer unsigned int to stream

        // // now set the sampler to the correct texture unit
        // glUniform1i(glGetUniformLocation(3, (name + number).c_str()), i);
        // // and finally bind the texture
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);

        // ------------------------------------------------------------------------------
        // glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // // retrieve texture number (the N in diffuse_textureN)
        // string number;
        // string name = textures[i].type;
        // if(name == "texture_diffuse")
        //     number = std::to_string(diffuseNr++);
        // else if(name == "texture_specular")
        //     number = std::to_string(specularNr++); // transfer unsigned int to stream
        // else if(name == "texture_normal")
        //     number = std::to_string(normalNr++); // transfer unsigned int to stream
        //     else if(name == "texture_height")
        //     number = std::to_string(heightNr++); // transfer unsigned int to stream

        // // now set the sampler to the correct texture unit
        // glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // // and finally bind the texture
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);



        // ------------------------------------------------------------------------------
        // TODO:: quitar todo lo hardcoded del cubo
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number = "1";
        string name = "texture_diffuse";

        // Indicar si se usan texturas
        glUniform1i(glGetUniformLocation(shaderID, "usesTextures"), 1);

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shaderID, (name + number).c_str()), i);
        // and finally bind the texture
        // glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
        glBindTexture(GL_TEXTURE_2D, texture->getProgramID());
    }
    
    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

}