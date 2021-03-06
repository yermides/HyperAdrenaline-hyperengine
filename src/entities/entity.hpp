#pragma once
#include <glm/glm.hpp>
#include <util/typealiases.hpp>
#include <util/macros.hpp>

struct Entity
{
    explicit Entity() = default;
    virtual ~Entity() = default;
    
    virtual void draw(glm::mat4 const& tranform) = 0;

    constexpr ProgramIdentifier getProgramID() const noexcept
        { return m_programID;  }
    constexpr void              setProgramID(ProgramIdentifier newID) noexcept
        { m_programID = newID; }
protected:
    ProgramIdentifier m_programID {0};
};
