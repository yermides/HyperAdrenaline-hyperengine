#include "r_material.hpp"

RMaterial::RMaterial(/* args */)
{
}

RMaterial::~RMaterial()
{
    LOG("[INFO]:: RMaterial deleted");
}

void 
RMaterial::loadMaterial( Cstring path )
{
    LOG("[INFO]:: He intentado crear un material.")
}

void 
RMaterial::loadFromFile( const std::string& path )
{
    this->setName(path);
    this->loadMaterial( path.c_str() );
}
