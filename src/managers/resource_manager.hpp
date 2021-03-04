#pragma once
#include <vector>
#include <algorithm>
#include <util/macros.hpp>
#include <resources/resource.hpp>

struct ResourceManager
{
    template<typename TResource>
    constexpr static TResource* getResource_t(std::string const& name) 
        { return get().getResource<TResource>(name);    }

    inline static void freeResource(std::string const& name)
        { return get().freeOne(name);                   }

    inline static void freeAllResources()
        { return get().freeAll();                       } 

protected:
    ResourceManager();
    ~ResourceManager();
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator=(ResourceManager const&) = delete;

    inline static ResourceManager& get() 
        { static ResourceManager instance; return instance; }

    template<typename TResource>
    TResource* getResource(std::string const& name)
    {
        Resource* newResource { nullptr };

        auto it = std::find_if(m_resources.begin(), m_resources.end(), [&](Resource* r) {
            return r->getName() == name;
        });

        // No encuentra el recurso, por ende lo crea
        if( it == m_resources.end() )
        {
            LOG("Resource not found, creating...");

            newResource = new TResource();
            newResource->loadFromFile(name);
            m_resources.push_back(newResource);
            return static_cast<TResource*>(newResource);
        }

        LOG("Resource found, returning...");

        return static_cast<TResource*>(*it);
    }

    inline void freeOne(std::string const& name)
    {
        auto it = std::find_if(m_resources.begin(), m_resources.end(), [&](Resource* r) {
            return r->getName() == name;
        });

        // Encuentra el recurso, por ende lo borra
        if( it != m_resources.end() )
        {
            LOG("[INFO]:: He encontrado el recurso a borrar.");
            m_resources.erase(it);
            delete *it;
        }
    }

    inline void freeAll() 
    {
        LOG(m_resources.size())

        for(auto r : m_resources) {
            if(r) { delete r; }
        }

        m_resources.clear();
    }

private:
    std::vector<Resource*> m_resources;
};
