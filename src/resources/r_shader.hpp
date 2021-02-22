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
    explicit RShader();
    ~RShader();

    // TODO:: separar las partes, los encuentra y compila de una
    ProgramIdentifier loadShaders( Cstring path_vertex, Cstring path_fragment );

    constexpr ProgramIdentifier getProgramID() const noexcept
		{ return m_programID; }
	constexpr void 				setProgramID(ProgramIdentifier newID) noexcept
		{ m_programID = newID; }
private:
    ProgramIdentifier m_programID;
};

