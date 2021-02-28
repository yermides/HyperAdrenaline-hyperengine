#include "r_mesh.hpp"

RMesh::RMesh()
: Resource{}
{
}

RMesh::~RMesh()
{
    for(auto m : m_meshes)
        delete m;
}

void 
RMesh::draw()
{
    for(auto m : m_meshes) 
        m->draw();
}

void 
RMesh::loadMesh(const std::string& filepath)
{
    // Cargar mallas con assimp
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
    
    LOG("Número de mallas: " << scene->mNumMeshes);

    m_meshes.reserve(scene->mNumMeshes);

    for(uint32_t i {0}; i < scene->mNumMeshes; ++i)
    {
        LOG("-- [BEGIN] loadMesh() --");

        // Crear una malla nueva
        auto mesh 	= new Mesh();
        auto amesh 	= scene->mMeshes[i];
        auto n2Elements = amesh->mNumVertices * 2;
        auto n3Elements = amesh->mNumVertices * 3;
        auto n4Faces    = amesh->mNumFaces * 4;

        mesh->m_vertices.reserve(n3Elements);
        mesh->m_normals.reserve(n3Elements);
        mesh->m_texture_coords.reserve(n2Elements);
        mesh->m_indices.reserve(n4Faces);           // asumimos que no hay más de cuatro vértices por cara

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

        // Almacenar los índices de las caras
        auto* indexarray        = amesh->mFaces;
        for(uint32_t j {0}; j < amesh->mNumFaces; ++j)
        {
            for(uint32_t k {0}; k < indexarray->mNumIndices; k++)
                mesh->m_indices.push_back(indexarray->mIndices[k]);

            ++indexarray;
        }

        // TODO:: materiales
        auto* materialsarray = scene->mMaterials[amesh->mMaterialIndex];
        LOG("Nombre del material: " << materialsarray->GetName().C_Str());
        
        LOG("Nombre de la malla: " << amesh->mName.C_Str());
        LOG("Posiciones: " << mesh->m_vertices.size());
        LOG("Normales: " << mesh->m_normals.size());
        LOG("Coordenadas de textura: " << mesh->m_texture_coords.size());
        LOG("Indices: " << mesh->m_indices.size());
        LOG("sizeof(mesh): " << 
            (mesh->m_vertices.size()*sizeof(GLfloat) )
        +   (mesh->m_normals.size()*sizeof(GLfloat) )
        +   (mesh->m_texture_coords.size()*sizeof(GLfloat) )
        +   (mesh->m_indices.size()*sizeof(GLuint) )
        +   (mesh->m_textures.size()*sizeof(Texture) )
        );

        // Inicializar valores de la malla en opengl
        mesh->initialize();

        // Guardar malla en el array
        m_meshes.push_back(mesh);

        LOG("-- [END] loadMesh() --");
    }
}

void 
RMesh::loadFromFile( const std::string& path )
{
    this->loadMesh(path);
}
