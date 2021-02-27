#pragma once
#include <util/typealiases.hpp>
#include <string>
#include <stdint.h>

struct Texture
{
    Texture(/* args */);
    ~Texture();

    uint32_t id;
    std::string type;
    std::string path;
};
