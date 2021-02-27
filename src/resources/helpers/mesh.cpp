#include <resources/helpers/mesh.hpp>

Mesh::Mesh(/* args */)
{
}

Mesh::~Mesh()
{
    // if(vbo) delete vbo;
    m_vertices.clear();
    m_normals.clear();
    m_texture_coords.clear();
    m_indexes.clear();
    m_textures.clear();
}

void 
Mesh::initialize(void)
{
    vbo = (GLuint*)malloc(3*sizeof(GLuint));
    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    // load data into vertex buffers
    // es posible que se necesiten más vbo's por cómo hace él la struct
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), &m_vertices.front(), GL_STATIC_DRAW);  

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(GLfloat), &m_normals.front(), GL_STATIC_DRAW); 

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glBufferData(GL_ARRAY_BUFFER, m_texture_coords.size() * sizeof(GLfloat), &m_texture_coords.front(), GL_STATIC_DRAW); 

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_normals.size() * sizeof(GLuint), &m_normals.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * sizeof(GLuint), &m_indexes.front(), GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);

    // Legacy
    /*
    {
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
    }
    */
}


void 
Mesh::draw() 
{
    // bind appropriate textures
    // unsigned int diffuseNr  = 1;
    // unsigned int specularNr = 1;
    // unsigned int normalNr   = 1;
    // unsigned int heightNr   = 1;

    // for(unsigned int i = 0; i < textures.size(); i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //     // retrieve texture number (the N in diffuse_textureN)
    //     string number;
    //     string name = textures[i].type;
    //     if(name == "texture_diffuse")
    //         number = std::to_string(diffuseNr++);
    //     else if(name == "texture_specular")
    //         number = std::to_string(specularNr++); // transfer unsigned int to stream
    //     else if(name == "texture_normal")
    //         number = std::to_string(normalNr++); // transfer unsigned int to stream
    //         else if(name == "texture_height")
    //         number = std::to_string(heightNr++); // transfer unsigned int to stream

    //     // now set the sampler to the correct texture unit
    //     glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    //     // and finally bind the texture
    //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
    // }
    
    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    // glActiveTexture(GL_TEXTURE0);

    // Legacy
    /*
    {
        // TODO:: terminar y revisar
        // Para cada buffer {
        // Asociar los datos en memoria al buffer   (glBufferData)
        // Enlazar el buffer con su attribute       (glBindBuffer)
        // indicar estructura del buffer            (glVertexAttribPointer)
        // Habilitar el buffer                      (glEnableVertexAttribArray)
        // }
        // Dibujar                                  (glDrawElements)

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
    */
}