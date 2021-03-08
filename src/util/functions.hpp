#pragma once
#include <string>

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

}