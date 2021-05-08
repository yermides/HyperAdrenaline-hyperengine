#include "r_mesh.hpp"

namespace hyper {

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
        delete m;
}

void 
RMesh::draw(RShader* const shader)
{
    if(!shader) return;

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
        LOG("-- [BEGIN] loadMesh() --");

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

        // Guardar vértices, normales y coordenadas de textura
        // Guardar índices
        auto* vertexarray       = amesh->mVertices;
        auto* normalsarray      = amesh->mNormals;
        auto* texcoordsarray    = amesh->mTextureCoords[0];

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
            if(!hasTextureCoords) continue;

            mesh->m_texture_coords.push_back(texcoordsarray->x);
            mesh->m_texture_coords.push_back(texcoordsarray->y);
            ++texcoordsarray;
        }

        // Huesos
        // auto bonearray = amesh->mBones[0];
        INFOLOG(amesh->mNumBones)
        for (uint32_t j {0}; j < amesh->mNumBones; ++j)
        {
            auto* bone = amesh->mBones[j];
            INFOLOG("bone name " << bone->mName.C_Str())

            INFOLOG("bone weight num " << bone->mNumWeights)

            for(uint32_t k {0}; k < bone->mNumWeights; ++k)
            {
                uint32_t id = bone->mWeights[j].mVertexId;
			    float weight = bone->mWeights[j].mWeight;
                INFOLOG("id" << VAR(id))
                INFOLOG("weight" << VAR(weight))
            }
            // ++bonearray;
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
        RMaterial* rmaterial = new RMaterial(amaterial, this->getDirectory());
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