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
    LOG("[INFO]:: RMaterial deleted");
}

void 
RMaterial::loadMaterial(std::string const& path)
{
    LOG("[INFO]:: He intentado crear un material.")
}

void 
RMaterial::loadFromFile(std::string const& path)
{
    this->setName(path);
    this->loadMaterial( path.c_str() );
}
