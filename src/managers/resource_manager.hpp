#pragma once
#include <vector>
#include <algorithm>
#include <util/macros.hpp>
#include <resources/resource.hpp>
// #include <resources/r_mesh.hpp>
// struct RMesh;
// struct RMaterial;
// struct RShader;
// struct RTexture;

struct ResourceManager
{
    template<typename TResource>
    constexpr static TResource* getResource_t(std::string const& name) 
        { return get().getResource<TResource>(name); }

    inline static void freeResource(std::string const& name)
        {return get().free(name);}

protected:
    ResourceManager();
    ~ResourceManager();
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

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

    inline void free(std::string const& name)
    {
        auto it = std::find_if(m_resources.begin(), m_resources.end(), [&](Resource* r) {
            return r->getName() == name;
        });

        // Encuentra el recurso, por ende lo borra
        if( it != m_resources.end() )
        {
            LOG("[INFO]:: He encontrado el recurso a borrar.");
            delete *it;
        }
    }

    // Test function to test templates
    // RMesh* getResourceMesh(std::string const& name);
private:
    std::vector<Resource*> m_resources;
};
