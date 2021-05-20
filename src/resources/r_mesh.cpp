#include "r_mesh.hpp"

namespace hyen {

RMesh::RMesh()
: Resource{}
{
}

RMesh::RMesh(std::string const& path)
: Resource{}
{
    this->loadFromFile(path);
}

RMesh::~RMesh()
{
    for(auto m : m_meshes)
    {
        delete m;
        m = nullptr;
    }

    m_meshes.clear();
}

void 
RMesh::draw(RShader* const shader)
{
    for(auto m : m_meshes) 
        m->draw(shader);
}

void 
RMesh::loadMesh(std::string const& filepath)
{
    // Cargar mallas con assimp
    Assimp::Importer importer;
    auto flags = 
        0
    |   aiProcess_Triangulate 
    |   aiProcess_FlipUVs 
    |   aiProcess_CalcTangentSpace
    // |   aiProcess_OptimizeMeshes
    // |   aiProcess_JoinIdenticalVertices
    // |   aiProcess_ImproveCacheLocality
    // |   aiProcess_OptimizeGraph
    // |   aiProcess_SplitLargeMeshes 
    // |   aiProcess_SortByPType
    ;
    auto scene = importer.ReadFile(filepath, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        ERRLOG(importer.GetErrorString());
    
    // LOG("Número de mallas: " << scene->mNumMeshes);

    m_meshes.reserve(scene->mNumMeshes);

    for(uint32_t i {0}; i < scene->mNumMeshes; ++i)
    {
        LOG("-- [BEGIN] loadMesh() -- ("  << VAR(i + 1) << " out of " VAR(scene->mNumMeshes) << ")" );

        // Crear una malla nueva
        auto mesh 	= new Mesh;
        auto amesh 	= scene->mMeshes[i];
        auto n2Elements = amesh->mNumVertices * 2;
        auto n3Elements = amesh->mNumVertices * 3;
        auto n4Faces    = amesh->mNumFaces * 4;

        mesh->m_vertices.reserve(n3Elements);
        mesh->m_normals.reserve(n3Elements);
        mesh->m_texture_coords.reserve(n2Elements);
        mesh->m_indices.reserve(n4Faces);           // asumimos que no hay más de cuatro vértices por cara
        mesh->m_tangents.reserve(n3Elements);
        mesh->m_bitangents.reserve(n3Elements);

        // Guardar vértices, normales y coordenadas de textura
        // Guardar índices
        auto* vertexarray       = amesh->mVertices;
        auto* normalsarray      = amesh->mNormals;
        auto* texcoordsarray    = amesh->mTextureCoords[0];
        auto* tangentarray      = amesh->mTangents;
        auto* bitangentarray    = amesh->mBitangents;

        bool hasTextureCoords = amesh->HasTextureCoords(0);

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
            
            // Almacenar coordenadas de textura
            if(hasTextureCoords)
            {
                mesh->m_texture_coords.push_back(texcoordsarray->x);
                mesh->m_texture_coords.push_back(texcoordsarray->y);
                ++texcoordsarray;

                mesh->m_tangents.push_back(tangentarray->x);
                mesh->m_tangents.push_back(tangentarray->y);
                mesh->m_tangents.push_back(tangentarray->z);
                ++tangentarray;

                mesh->m_bitangents.push_back(bitangentarray->x);
                mesh->m_bitangents.push_back(bitangentarray->y);
                mesh->m_bitangents.push_back(bitangentarray->z);
                ++bitangentarray;
            }
            else
            {
                mesh->m_texture_coords.push_back(0.0f);
                mesh->m_texture_coords.push_back(0.0f);
            }
        }

        // Almacenar los índices de las caras
        auto* indexarray        = amesh->mFaces;
        for(uint32_t j {0}; j < amesh->mNumFaces; ++j)
        {
            for(uint32_t k {0}; k < indexarray->mNumIndices; k++)
                mesh->m_indices.push_back(indexarray->mIndices[k]);

            ++indexarray;
        }

        // Resize to the size to skip possible 4th vertex
        mesh->m_indices.resize(mesh->m_indices.size());

        // Guardar texturas y materiales
        // TODO:: mejorar los materiales (irse al RMaterial::draw() y considerar más texturas aparte de la difusa)
        auto* amaterial = scene->mMaterials[amesh->mMaterialIndex];
        RMaterial* rmaterial = new RMaterial(amaterial, getDirectory());
        mesh->m_materials.push_back(rmaterial);

        // Inicializar valores de la malla en opengl
        mesh->initialize();

        // Guardar malla en el array
        m_meshes.push_back(mesh);

        LOG("-- [END] loadMesh() --");
    }
}

void 
RMesh::loadFromFile(std::string const& path)
{
    this->setName(path);
    this->loadMesh(path);
}

}