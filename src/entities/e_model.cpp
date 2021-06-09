#include "e_model.hpp"

namespace hyen {

EModel::EModel() 
: Entity{}
{
}

EModel::EModel(std::string const& path)
{
    loadFromFile(path);
}

EModel::~EModel()
{
}

void 
EModel::draw(glm::mat4 const& transform)
{
    // Use our shader
    m_shader->bind();

    // Pass the model matrix
    m_shader->setMat4("model", transform);

    // Draw the mesh now that everything is setup
    m_rmesh->draw(m_shader);

    // Unbind shader, if other shader is being used for other models
    m_shader->unbind();
}

void 
EModel::loadFromFile(std::string const& path)
{
    m_rmesh = ResourceManager::getResource_t<RMesh>(path);
}

std::vector<float> const
EModel::getVertexPositions(void)
{
    std::vector<float> vertices;

    for(auto mesh : m_rmesh->m_meshes) { 
        vertices.insert( vertices.end(), mesh->m_vertices.begin(), mesh->m_vertices.end() );
    }

    return vertices;
}

std::vector<float> const 
EModel::getVertexNormals(void)
{
    std::vector<float> normals;

    for(auto mesh : m_rmesh->m_meshes) { 
        normals.insert( normals.end(), mesh->m_normals.begin(), mesh->m_normals.end() );
    }

    return normals;
}

std::vector<float> const 
EModel::getVertexIndices(void)
{
    std::vector<float> indices;

    for(auto mesh : m_rmesh->m_meshes) { 
        indices.insert( indices.end(), mesh->m_indices.begin(), mesh->m_indices.end() );
    }

    return indices;
}

}