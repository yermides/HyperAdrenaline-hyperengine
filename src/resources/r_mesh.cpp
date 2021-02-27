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
    // LOG("m_meshes.size():: " << m_meshes.size());
    for(auto m : m_meshes) 
        m->draw();
}

void 
RMesh::loadMesh(const std::string& filepath)
{
    std::vector<int> is = {1,1,1,1,1,1,1,1,1};
    int is2[9] = {1,1,1,1,1,1,1,1,1};

    LOG(sizeof(is) << ":::" << sizeof(is2) <<":::"<< sizeof(*is.data()));

    // this->loadMesh_ejemploCubo(filepath); // TODO:: Comentar
    // return;

    // TODO:: cargar mallas con assimp
    Assimp::Importer importer;
    auto flags = 
        0
    |   aiProcess_Triangulate 
    |   aiProcess_FlipUVs 
    |   aiProcess_CalcTangentSpace
    // |   aiProcess_GenSmoothNormals 
    ;
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
        auto n4Faces    = amesh->mNumFaces * 4;

        mesh->m_vertices.reserve(n3Elements);
        mesh->m_normals.reserve(n3Elements);
        mesh->m_texture_coords.reserve(n2Elements);
        mesh->m_indexes.reserve(n4Faces);           // asumimos que no hay más de cuatro vértices por cara

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
            for(uint32_t k {0}; k < indexarray->mNumIndices; k++)
                mesh->m_indexes.push_back(indexarray->mIndices[k]);

            // mesh->m_indexes.push_back(indexarray->mIndices[0]);
            // mesh->m_indexes.push_back(indexarray->mIndices[1]);
            // mesh->m_indexes.push_back(indexarray->mIndices[2]);
            ++indexarray;
        }
        
        LOG("Vertices values:: " << mesh->m_vertices.size());
        LOG("Normals values:: " << mesh->m_normals.size());
        LOG("TexCoords values:: " << mesh->m_texture_coords.size());
        LOG("Indices values:: " << mesh->m_indexes.size());


        LOG("SIZEOF(MESH):: " << 
            (mesh->m_vertices.size()*sizeof(GLfloat) )
        +   (mesh->m_normals.size()*sizeof(GLfloat) )
        +   (mesh->m_texture_coords.size()*sizeof(GLfloat) )
        +   (mesh->m_indexes.size()*sizeof(GLuint) )
        +   (mesh->m_textures.size()*sizeof(Texture) )
        );

        LOG("SIZEOF(vertices):: " << sizeof(mesh->m_vertices));
        LOG("SIZEOF(vectorint):: " << sizeof(std::vector<int>));
        LOG("SIZEOF(vectorunsignedint):: " << sizeof(std::vector<unsigned int>));
        LOG("SIZEOF(vectorfloat):: " << sizeof(std::vector<float>));

        // Inicializar valores de la malla en opengl
        mesh->initialize();

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
RMesh::loadFromFile( const std::string& path )
{
    this->loadMesh(path);
}
