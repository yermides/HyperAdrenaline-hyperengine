#pragma once
#include <string>
#include <util/typealiases.hpp>

struct Resource
{
    explicit Resource() {};
    virtual ~Resource() {};

    constexpr   const std::string&  getName( void ) const noexcept
        { return m_name;    }

    inline      void                setName( const std::string& name ) noexcept
        { m_name = name;    }
        
    virtual     void                loadFromFile( const std::string& path ) {};
protected:
    std::string m_name {"default"};
};
