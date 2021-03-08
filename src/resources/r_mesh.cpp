#include "r_mesh.hpp"

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
RMesh::draw(ProgramIdentifier const shaderID)
{
    for(auto m : m_meshes) 
        m->draw(shaderID);
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
    // |   aiProcess_GenSmoothNormals 
    ;
    auto scene = importer.ReadFile(filepath, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        ERRLOG(importer.GetErrorString());
    
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
            
            // Almacenar coordenadas de textura
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

        // Resize to the size to skip possible 4th vertex
        mesh->m_indices.resize(mesh->m_indices.size());

        // ----------------------------------------------------------------------------------------------
        // --------------------------Danger zone: materiales in progress---------------------------------
        // ----------------------------------------------------------------------------------------------
        // TODO:: materiales
        auto* amaterial = scene->mMaterials[amesh->mMaterialIndex];

        // glm::vec3 diffuse_color;
        // auto res = amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, &diffuse_color.x, NULL);

        // aiString str1;
        // amaterial->GetTexture((aiTextureType)11, 0, &str1);
        // INFOLOG("xxxxxxxx: "<<VAR(str1.C_Str()))

        // if(res == aiReturn_SUCCESS)
        //     INFOLOG("Diffuse color: "<<VAR(diffuse_color.x) << VAR(diffuse_color.y) <<VAR(diffuse_color.z) );
            
        // INFOLOG("Número de propiedades: "<< VAR(amaterial->mNumProperties));

        for(uint32_t j {0}; j<amaterial->mNumProperties; ++j)
        {
            auto property = amaterial->mProperties[j];
            // INFOLOG("Propiedad " << VAR(property->mKey.C_Str()) <<":"<< VAR(property->mData) <<":"<< VAR( property->mType ) );
        }

        for (int j {0}; j <= AI_TEXTURE_TYPE_MAX; j++)
        {
            // LOG("Numero de propiedades del material: {" << amaterial->mNumProperties << "}")
            // auto properties = amaterial->mProperties[1];
            // LOG(properties->mKey.C_Str());

            // for (uint32_t m = 0; m < amaterial->mNumProperties; m++)
            // {
            // }
            
            auto type = (aiTextureType)j;
            auto count = amaterial->GetTextureCount(type);
            // LOG("Texturas de tipo [" << type << "] = " << count );

            // análogo de [loadMaterialTextures] solo que los pongo a lo bruto en el array
            // TODO:: cargar todo tipo de texturas y ser capaz de interpretarlas (Mesh::draw())
            if(type == aiTextureType_DIFFUSE)
            {
                for (size_t k = 0; k < count; k++)
                {
                    RMaterial* rmaterial = new RMaterial();
                    rmaterial->loadMaterial(amaterial);
                    delete rmaterial;
                    // Crear y guardar una textura en el array
                    aiString str;
                    amaterial->GetTexture(type, k, &str);

                    std::string formattedpath = str.C_Str();
                    util::replaceCharacters(formattedpath, "\\\\", "/");

                    auto filepath = getDirectory() +"/"+ formattedpath;
                    auto texture  = ResourceManager::getResource_t<RTexture>(filepath);
                    // RTexture* texture = new RTexture(filepath);
                    // texture->initialize(); // voy a probar a hacer esto en el draw()
                    mesh->m_textures.push_back(texture);
                }
            }


        }

        LOG("Nombre del material: " << amaterial->GetName().C_Str());
        LOG("Nombre de la malla: " << amesh->mName.C_Str());
        LOG("Posiciones: " << mesh->m_vertices.size());
        LOG("Normales: " << mesh->m_normals.size());
        LOG("Coordenadas de textura: " << mesh->m_texture_coords.size());
        LOG("Indices: " << mesh->m_indices.size());
        LOG("Texturas: " << mesh->m_textures.size());
        
        LOG("sizeof(mesh): " << 
            (mesh->m_vertices.size()*sizeof(GLfloat) )
        +   (mesh->m_normals.size()*sizeof(GLfloat) )
        +   (mesh->m_texture_coords.size()*sizeof(GLfloat) )
        +   (mesh->m_indices.size()*sizeof(GLuint) )
        +   (mesh->m_textures.size()*sizeof(Texture) )
        );

        // ----------------------------------------------------------------------------------------------
        // -----------------------(end) Danger zone: materiales in progress------------------------------
        // ----------------------------------------------------------------------------------------------

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
