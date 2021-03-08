#include "r_material.hpp"

RMaterial::RMaterial()
: Resource{}
{
}

RMaterial::RMaterial(std::string const& path)
: Resource{}
{
    this->loadFromFile(path);
}

RMaterial::~RMaterial()
{
    INFOLOG("RMaterial deleted");

    m_mapKa = nullptr;
    m_mapKd = nullptr;
    m_mapKs = nullptr;
}

void 
RMaterial::loadMaterial(std::string const& path)
{
    INFOLOG("He intentado crear un material.")
}

void 
RMaterial::loadMaterial(const aiMaterial* const amaterial)
{
    aiReturn res;
    res = amaterial->Get(AI_MATKEY_COLOR_AMBIENT, &m_Ka.r, NULL);   // .x por ser el principio de los datos del array
    if(res != aiReturn_SUCCESS) m_Ka = {0.0f, 0.0f, 0.0f};
    res = amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, &m_Kd.r, NULL);
    if(res != aiReturn_SUCCESS) m_Kd = {0.0f, 0.0f, 0.0f};
    res = amaterial->Get(AI_MATKEY_COLOR_SPECULAR, &m_Ks.r, NULL);
    if(res != aiReturn_SUCCESS) m_Ks = {0.0f, 0.0f, 0.0f};
    res = amaterial->Get(AI_MATKEY_SHININESS, &m_Ns, NULL);
    if(res != aiReturn_SUCCESS) m_Ns = 0.0f;
    res = amaterial->Get(AI_MATKEY_OPACITY, &m_d, NULL);
    if(res != aiReturn_SUCCESS) m_d = 0.0f;

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
