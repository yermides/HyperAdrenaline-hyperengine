#pragma once
#include <glm/glm.hpp>
#include <resources/r_shader.hpp>
#include <util/typealiases.hpp>
#include <util/macros.hpp>

namespace hyen {

struct Entity
{
    explicit Entity() = default;
    virtual ~Entity() = default;
    
    virtual void draw(glm::mat4 const& tranform) = 0;

    constexpr RShader* const    getShader(void) const noexcept
        { return m_shader;      }
    constexpr void              setShader(RShader* const shader) noexcept
        { m_shader = shader;    }
protected:
    RShader* m_shader               { nullptr   };
};

}
