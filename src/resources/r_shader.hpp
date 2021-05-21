#pragma once
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "resource.hpp"
#include <shaders/shaders.hpp>

namespace hyen {

struct RShader : public Resource
{
    explicit RShader();
    explicit RShader(std::string const& path);
    explicit RShader(Cstring path_vertex, Cstring path_fragment, Cstring path_geometry = nullptr );
    ~RShader();

    // Cargar desde archivo .vs, .fs
    ProgramIdentifier loadShaders( Cstring path_vertex, Cstring path_fragment );
    ProgramIdentifier loadShadersFromMemory( Cstring path_vertex, Cstring path_fragment, Cstring path_geometry = nullptr );
    void loadFromFile( std::string const& path ) final;

    inline void bind() 
        { glUseProgram(m_programID);                                                                    }

    inline void unbind() 
        { glUseProgram(0);                                                                              }

    inline void setBool(const std::string &name, bool value) const
        { glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);                     }

    inline void setInt(const std::string &name, int value) const
        { glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);                          }

    inline void setFloat(const std::string &name, float value) const
        { glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);                          }

    inline void setVec2(const std::string &name, const glm::vec2 &value) const
        { glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);                  }

    inline void setVec2(const std::string &name, float x, float y) const
        { glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);                           }

    inline void setVec3(const std::string &name, const glm::vec3 &value) const
        { glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);                  }

    inline void setVec3(const std::string &name, float x, float y, float z) const
        { glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);                        }

    inline void setVec4(const std::string &name, const glm::vec4 &value) const
        { glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);                  }

    inline void setVec4(const std::string &name, float x, float y, float z, float w) 
        { glUniform4f(glGetUniformLocation(m_programID, name.c_str()), x, y, z, w);                     }

    inline void setMat2(const std::string &name, const glm::mat2 &mat) const
        { glUniformMatrix2fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    inline void setMat3(const std::string &name, const glm::mat3 &mat) const
        { glUniformMatrix3fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    inline void setMat4(const std::string &name, const glm::mat4 &mat) const
        { glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    constexpr ProgramIdentifier getProgramID() const noexcept
		{ return m_programID;   }

	constexpr void 				setProgramID(ProgramIdentifier newID) noexcept
		{ m_programID = newID;  }
private:
    ProgramIdentifier m_programID { 0 };
};

}
