#include <vector>
#include <algorithm>
#include <resources/r_mesh.hpp>
#include <resources/r_texture.hpp>
#include <resources/r_shader.hpp>
#include <resources/r_material.hpp>

class ResourceManager
{
    ResourceManager(/* args */);
    ~ResourceManager();

    Resource* getResource( const std::string& name );
    
private:
    std::vector<Resource*> m_resources;
};
