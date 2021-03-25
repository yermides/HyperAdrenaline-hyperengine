#pragma once
#include <glm/glm.hpp>
#include <resources/r_shader.hpp>
#include <util/typealiases.hpp>
#include <util/macros.hpp>

namespace hyper {

struct Entity
{
    explicit Entity() = default;
    virtual ~Entity() = default;
    
    virtual void draw(glm::mat4 const& tranform) = 0;

    // constexpr ProgramIdentifier getProgramID(void) const noexcept
    //     { return m_programID;   }
    // constexpr void              setProgramID(ProgramIdentifier newID) noexcept
    //     { m_programID = newID;  }

    constexpr RShader* const    getShader(void) const noexcept
        { return m_shader;      }
    constexpr void              setShader(RShader* const shader) noexcept
        { m_shader = shader;    }
protected:
    RShader* m_shader               { nullptr   };
    // ProgramIdentifier m_programID   { 0         };
};

}
