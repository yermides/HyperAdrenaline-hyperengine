#include "r_mesh.hpp"

RMesh::RMesh()
: Resource{}
{
}

RMesh::~RMesh()
{
    glDeleteBuffers(1, &vbo_position);
    glDeleteBuffers(1, &vbo_color);
    glDeleteVertexArrays(1, &vao);
}

void 
RMesh::draw()
{
    // this->draw_ejemploCubo(); // TODO:: Comentar
    // return;

    for(auto m : m_meshes) 
        m->draw();
}

void 
RMesh::draw_ejemploCubo()
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, m_nVertex);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void 
RMesh::loadMesh(const std::string& filepath)
{
    // this->loadMesh_ejemploCubo(filepath); // TODO:: Comentar
    // return;

    // TODO:: cargar mallas con assimp
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    auto scene = importer.ReadFile(filepath, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        LOG("[ERROR]:: " << importer.GetErrorString());
    
    LOG("MESH NUM:: " << scene->mNumMeshes);

    m_meshes.reserve(scene->mNumMeshes);

    for(uint32_t i {0}; i < scene->mNumMeshes; ++i)
    {
        LOG("-- [BEGIN] Allocate new Mesh() --");

        // Crear una malla nueva
        auto mesh 	= new Mesh();
        auto amesh 	= scene->mMeshes[i];
        auto n2Elements = amesh->mNumVertices * 2;
        auto n3Elements = amesh->mNumVertices * 3;
        auto n3Faces    = amesh->mNumFaces * 3;

        mesh->m_vertices.reserve(n3Elements);
        mesh->m_normals.reserve(n3Elements);
        mesh->m_texture_coords.reserve(n2Elements);
        mesh->m_indexes.reserve(n3Faces);

        LOG("NAME::	" << amesh->mName.C_Str());
        LOG("FACES:: " << amesh->mNumFaces);
        LOG("NUMBER OF ELEMENTS IN VERTEX ARRAY:: " << n3Elements);
        LOG("NUMBER OF ELEMENTS IN INDEX ARRAY:: " << n3Faces);

        // Guardar vértices, normales y coordenadas de textura
        // Guardar índices
        // TODO:: guardar texturas y materiales
        auto* vertexarray       = amesh->mVertices;
        auto* normalsarray      = amesh->mNormals;
        auto* texcoordsarray    = amesh->mTextureCoords[0];

        for(uint32_t j {0}; j < amesh->mNumVertices; ++j)
        {
            // Almacenar posiciones de vértices
            mesh->m_vertices.push_back(vertexarray->x);
            mesh->m_vertices.push_back(vertexarray->y);
            mesh->m_vertices.push_back(vertexarray->z);
            ++vertexarray;
            
            // Almacenar normales de los vértices
            mesh->m_normals.push_back(normalsarray->x);
            mesh->m_normals.push_back(normalsarray->y);
            mesh->m_normals.push_back(normalsarray->z);
            ++normalsarray;
            
            // Almacenar coordenadas de textura ¡Duda!
            mesh->m_texture_coords.push_back(texcoordsarray->x);
            mesh->m_texture_coords.push_back(texcoordsarray->y);
            ++texcoordsarray;
        }

        // Duda de si usar el mNumIndices para recorrer el array
        auto* indexarray        = amesh->mFaces;
        for(uint32_t j {0}; j < amesh->mNumFaces; ++j)
        {
            // Almacenar los índices de las caras
            mesh->m_indexes.push_back(indexarray->mIndices[0]);
            mesh->m_indexes.push_back(indexarray->mIndices[1]);
            mesh->m_indexes.push_back(indexarray->mIndices[2]);
            ++indexarray;
        }
        
        LOG("Vertices values:: " << mesh->m_vertices.size());
        LOG("Normals values:: " << mesh->m_normals.size());
        LOG("TexCoords values:: " << mesh->m_texture_coords.size());
        LOG("Indices values:: " << mesh->m_indexes.size());

        // Guardar malla en el array
        m_meshes.push_back(mesh);

        // Antes de terminar, voy a tratar de crear los handles de vao y vbo
        // No sé si hay que crearlos por cada mesh o fuera, pero allá vá
        // Como de momento solo es una, debería funcionar
        // Vale, creo que todo esto va en Mesh::draw()


        LOG("-- [END] Allocate new Mesh() --");
    }

}

void 
RMesh::loadMesh_ejemploCubo(const std::string& filepath)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = { 
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    this->m_nVertex = 12*3;
}

void 
RMesh::loadFromFile( const std::string& path )
{
    this->loadMesh(path);
}
