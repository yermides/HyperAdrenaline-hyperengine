#include "r_material.hpp"

namespace hyper {

RMaterial::RMaterial()
: Resource{}
{
    // Default material
    m_Ka = default_material_ambient_color;
    m_Kd = default_material_diffuse_color;
    m_Ks = default_material_specular_color;
    m_Ns = default_material_specular_exponent;
    m_d = default_material_opacity;
}

RMaterial::RMaterial(std::string const& path)
: Resource{}
{
    this->loadFromFile(path);
}

RMaterial::RMaterial(aiMaterial* amaterial, std::string const& search_directory)
{
    this->loadMaterial(amaterial, search_directory);
}

RMaterial::~RMaterial()
{
    INFOLOG("RMaterial deleted");

    m_mapKa = nullptr;
    m_mapKd = nullptr;
    m_mapKs = nullptr;
}

void 
RMaterial::initialize(void)
{
    if(m_mapKa) m_mapKa->initialize();
    if(m_mapKd) m_mapKd->initialize();
    if(m_mapKs) m_mapKs->initialize();
}

void 
RMaterial::loadMaterial(std::string const& path)
{
    INFOLOG("He intentado crear un material.")
}

void 
RMaterial::loadMaterial(aiMaterial* amaterial, std::string const& search_directory)
{
    INFOLOG("RMaterial::loadMaterial search_directory " << VAR(search_directory) );

    aiReturn res;
    res = amaterial->Get(AI_MATKEY_COLOR_AMBIENT, &m_Ka.r, NULL);   // .x por ser el principio de los datos del array
    if(res != aiReturn_SUCCESS) m_Ka = default_material_ambient_color;
    res = amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, &m_Kd.r, NULL);
    if(res != aiReturn_SUCCESS) m_Kd = default_material_diffuse_color;
    res = amaterial->Get(AI_MATKEY_COLOR_SPECULAR, &m_Ks.r, NULL);
    if(res != aiReturn_SUCCESS) m_Ks = default_material_specular_color;
    res = amaterial->Get(AI_MATKEY_SHININESS, &m_Ns, NULL);
    if(res != aiReturn_SUCCESS) m_Ns = default_material_specular_exponent;
    res = amaterial->Get(AI_MATKEY_OPACITY, &m_d, NULL);
    if(res != aiReturn_SUCCESS) m_d = default_material_opacity;


    // Conseguir texturas
    aiString str;
    
    // Ambiente
    res = amaterial->GetTexture(aiTextureType_AMBIENT, 0, &str);

    if(res == aiReturn_SUCCESS)
    {
        std::string formattedpath = str.C_Str();
        util::replaceCharacters(formattedpath, "\\\\", "/");
        m_mapKa = ResourceManager::getResource_t<RTexture>(search_directory +"/"+ formattedpath);
    }

    // Difusa
    res = amaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);

    if(res == aiReturn_SUCCESS)
    {
        std::string formattedpath = str.C_Str();
        util::replaceCharacters(formattedpath, "\\\\", "/");
        m_mapKd = ResourceManager::getResource_t<RTexture>(search_directory +"/"+ formattedpath);
    }

    // Especular
    res = amaterial->GetTexture(aiTextureType_SPECULAR, 0, &str);

    if(res == aiReturn_SUCCESS)
    {
        std::string formattedpath = str.C_Str();
        util::replaceCharacters(formattedpath, "\\\\", "/");
        m_mapKs = ResourceManager::getResource_t<RTexture>(search_directory +"/"+ formattedpath);
    }

    INFOLOG("RMaterial name " << VAR(amaterial->GetName().C_Str()) );

    INFOLOG("\n" << "Propiedades del material: " << "\n"
        << VAR(m_Ka.r) << ":" << VAR(m_Ka.g) << ":" << VAR(m_Ka.b) << "\n"
        << VAR(m_Kd.r) << ":" << VAR(m_Kd.g) << ":" << VAR(m_Kd.b) << "\n"
        << VAR(m_Ks.r) << ":" << VAR(m_Ks.g) << ":" << VAR(m_Ks.b) << "\n"
        << VAR(m_Ns) << "\n"
        << VAR(m_d) << "\n"
    );
}

void 
RMaterial::loadFromFile(std::string const& path)
{
    this->setName(path);
    this->loadMaterial( path.c_str() );
}

}