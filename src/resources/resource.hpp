#pragma once
#include <string>
#include <util/typealiases.hpp>

struct Resource
{
    explicit Resource() {};
    virtual ~Resource() {};

    inline const std::string&  getName( void ) const noexcept
        { return m_name;    }

    inline void                setName( const std::string& name ) noexcept
        { m_name = name;    }
        
private:
    std::string m_name;
};
