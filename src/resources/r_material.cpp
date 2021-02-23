#include "r_material.hpp"

RMaterial::RMaterial(/* args */)
{
}

RMaterial::~RMaterial()
{
}

void 
RMaterial::loadMaterial( Cstring path )
{
}

void 
RMaterial::loadFromFile( const std::string& path )
{
    this->loadMaterial( path.c_str() );
}
