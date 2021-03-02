#include <managers/resource_manager.hpp>

ResourceManager::ResourceManager(/* args */)
{
}

ResourceManager::~ResourceManager()
{
}

// Test function to test templates
// RMesh* 
// ResourceManager::getResourceMesh(std::string const& name)
// {
//     RMesh* newResource { nullptr };

//     auto it = std::find_if(m_resources.begin(), m_resources.end(), [&](Resource* r) {
//         return r->getName() == name;
//     });

//     // No encuentra el recurso, por ende lo crea
//     if( it == m_resources.end() )
//     {
//         LOG("Resource not found, creating...");
//         newResource = new RMesh();
//         // Por eso tiene que ser una función virtual
//         // TODO::
//         newResource->loadFromFile(name);
//         m_resources.push_back( static_cast<Resource*>(newResource));
//         return newResource;
//     }

//     LOG("Resource found, returning...");

//     return static_cast<RMesh*>(*it);
// }
