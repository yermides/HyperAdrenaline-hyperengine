#pragma once
#include <string>
#include <bullet/LinearMath/btVector3.h>

namespace hyen {

namespace util {

inline static void replaceCharacters(std::string& toformat, std::string const& oldcharacters, std::string const& newcharacters)
{
    size_t pos = toformat.find(oldcharacters);
    while( pos != std::string::npos)
    {
        toformat.replace(pos, newcharacters.size()+1, newcharacters);
        pos = toformat.find(oldcharacters, pos);
    }
}

inline static std::string getCurrentFolderName(std::string const& toformat)
{
    auto pos = toformat.find_last_of("/") + 1;
    return std::move(toformat.substr(pos));
}

inline static std::string getCurrentFileExtension(std::string const& toformat)
{
    auto pos = toformat.find_last_of(".") + 1;
    return std::move(toformat.substr(pos));
}

inline static btVector3 glmVec3TobtVec3(glm::vec3 const& vec3)
{
    return std::move( btVector3(vec3.x, vec3.y,vec3.z) );
}

inline static glm::vec3 btVec3ToGlmVec3(btVector3 const& vec3)
{
    return std::move( glm::vec3(vec3.getX(), vec3.getY(), vec3.getZ()) );
}

}

}