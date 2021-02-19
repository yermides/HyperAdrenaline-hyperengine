#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "resource.hpp"

struct RShader : public Resource
{
    explicit RShader(/* args */);
    ~RShader();

    // TODO:: separar las partes, los encuentra y compila de una
    static ProgramIdentifier loadShaders( Cstring path_vertex, Cstring path_fragment );
};

