#include "r_material.hpp"

namespace hyen {

RMaterial::RMaterial()
:   Resource{}
,   m_Ka(default_material_ambient_color)
,   m_Kd(default_material_diffuse_color)
,   m_Ks(default_material_specular_color)
,   m_Ns(default_material_specular_exponent)
,   m_d(default_material_opacity)
{
}

RMaterial::RMaterial(std::string const& path)
:   Resource{}
,   m_Ka(default_material_ambient_color)
,   m_Kd(default_material_diffuse_color)
,   m_Ks(default_material_specular_color)
,   m_Ns(default_material_specular_exponent)
,   m_d(default_material_opacity)
{
    loadFromFile(path);
}

RMaterial::RMaterial(aiMaterial* amaterial, std::string const& search_directory)
:   Resource{}
,   m_Ka(default_material_ambient_color)
,   m_Kd(default_material_diffuse_color)
,   m_Ks(default_material_specular_color)
,   m_Ns(default_material_specular_exponent)
,   m_d(default_material_opacity)
{
    INFOLOG("RMaterial initialized");
    loadMaterial(amaterial, search_directory);
}

RMaterial::~RMaterial()
{
    INFOLOG("RMaterial deleted");

    m_mapKa = nullptr;
    m_mapKd = nullptr;
    m_mapKs = nullptr;
    m_mapKn = nullptr;
}

void 
RMaterial::initialize(void)
{
    if(m_mapKa) m_mapKa->initialize();
    if(m_mapKd) m_mapKd->initialize();
    if(m_mapKs) m_mapKs->initialize();
    if(m_mapKn) m_mapKn->initialize();
}

void 
RMaterial::draw(RShader* const shader)
{
    // Aumentar el indice de textura para cada una que se le pase al shader
    int i {0};

    shader->setVec3("material.ambientColor", m_Ka);
    shader->setVec3("material.diffuseColor", m_Kd);
    shader->setVec3("material.specularColor", m_Ks);
    shader->setFloat("material.shininess", m_Ns);
    shader->setFloat("material.opacity", m_d);

    // Si hay ambient texture, de momento sin usar en el shader
    // if(m_mapKa)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //     // retrieve texture number (the N in diffuse_textureN)
    //     std::string name = "material.texture_ambient";
    //     std::string number = "1";

    //     // Indicar si se usan texturas (sí)
    //     shader->setInt("material.usesAmbientTexture", 1);

    //     // now set the sampler to the correct texture unit
    //     shader->setInt((name + number).c_str(), i);
    //     // and finally bind the texture
    //     glBindTexture(GL_TEXTURE_2D, m_mapKa->getProgramID());
    //     ++i;
    // }
    // else
    // {
    //     // Indicar si se usan texturas (no)
    //     shader->setInt("material.usesAmbientTexture", 0);
    // }

    // Si hay textura difusa
    if(m_mapKd)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        shader->setInt("material.usesDiffuseTexture", 1);
        shader->setInt("material.diffuse", i);
        glBindTexture(GL_TEXTURE_2D, m_mapKd->getProgramID());
        ++i;
    }
    else
    {
        shader->setInt("material.usesDiffuseTexture", 0);
    }

    // Si hay specular texture
    if(m_mapKs)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        shader->setInt("material.usesSpecularTexture", 1);
        shader->setInt("material.specular", i);
        glBindTexture(GL_TEXTURE_2D, m_mapKs->getProgramID());
        ++i;
    }
    else
    {
        shader->setInt("material.usesSpecularTexture", 0);
    }

    // Si hay specular texture, de momento en progreso de usar en el nuevo shader
    // if(m_mapKn)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //     shader->setInt("material.usesNormalTexture", 1);
    //     shader->setInt("material.normal", i);
    //     glBindTexture(GL_TEXTURE_2D, m_mapKn->getProgramID());
    //     ++i;
    // }
    // else
    // {
    //     shader->setInt("material.usesNormalTexture", 0);
    // }
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

    // Normales. Nota: assimp no reconoce map_Bump, renombrar a mapKn dentro del .mtl
    res = amaterial->GetTexture(aiTextureType_NORMALS, 0, &str);

    if(res == aiReturn_SUCCESS)
    {
        std::string formattedpath = str.C_Str();
        util::replaceCharacters(formattedpath, "\\\\", "/");
        m_mapKn = ResourceManager::getResource_t<RTexture>(search_directory +"/"+ formattedpath);
    }


    // INFOLOG("RMaterial name " << VAR(amaterial->GetName().C_Str()) );

    // INFOLOG("\n" << "Propiedades del material: " << "\n"
    //     << VAR(m_Ka.r) << ":" << VAR(m_Ka.g) << ":" << VAR(m_Ka.b) << "\n"
    //     << VAR(m_Kd.r) << ":" << VAR(m_Kd.g) << ":" << VAR(m_Kd.b) << "\n"
    //     << VAR(m_Ks.r) << ":" << VAR(m_Ks.g) << ":" << VAR(m_Ks.b) << "\n"
    //     << VAR(m_Ns) << "\n"
    //     << VAR(m_d) << "\n"
    // );
}

void 
RMaterial::loadFromFile(std::string const& path)
{
    this->setName(path);
    this->loadMaterial( path.c_str() );
}

}