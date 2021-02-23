#include <managers/resource_manager.hpp>

ResourceManager::ResourceManager(/* args */)
{
}

ResourceManager::~ResourceManager()
{
}

Resource* 
ResourceManager::getResource( const std::string& name )
{
    Resource* newResource { nullptr };

    auto it = std::find_if(m_resources.begin(), m_resources.end(), [&](Resource* r) {
        return r->getName() == name;
    });

    // No encuentra el recurso, por ende lo crea
    if( it == m_resources.end() )
    {
        newResource = new Resource();
        // Por eso tiene que ser una función virtual
        // TODO::
        newResource->loadFromFile(name);
        m_resources.push_back(newResource);
        return newResource;
    }
        
    return *it;
}
