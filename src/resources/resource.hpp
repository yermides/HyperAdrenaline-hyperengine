#pragma once
#include <string>
#include <util/typealiases.hpp>
#include <util/macros.hpp>

constexpr Cstring define_default_resource_name { "" };

struct Resource
{
    explicit Resource() {};
    virtual ~Resource() {};

    // Resources are identified by their full relative route (example: assets/cube.obj)
    constexpr   std::string const&  getName( void ) const noexcept
        { return m_name;    }

    inline      void                setName( const std::string& name ) noexcept
        { m_name = name;    }

    // Obtains the route without the last '/'
    inline      std::string const   getDirectory()  const noexcept
        { 
            auto i = m_name.find_last_of('/');

            if(i >=0 && i <= m_name.length())
                return m_name.substr(0, i); 

            return "";
        }

    inline      std::string const   getFilename()   const noexcept
        { 
            auto i = m_name.find_last_of('/');

            if(i >=0 && i <= m_name.length())
                return m_name.substr(i+1); 

            return m_name; 
        };
        
    virtual     void                loadFromFile( const std::string& path ) {};
protected:
    std::string m_name { define_default_resource_name };
};
