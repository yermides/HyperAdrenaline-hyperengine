#include "e_model.hpp"

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
    glUseProgram(m_programID);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(
        glGetUniformLocation(m_programID, "MVP")
        , 1
        , GL_FALSE
        , &transform[0][0]
    );

    // Draw the mesh now that everything is setup
    m_rmesh->draw(m_programID);

    // Unbind shader, if other shader is being used for other models
    glUseProgram(0);
}

void 
EModel::loadFromFile(std::string const& path)
{
    m_rmesh = ResourceManager::getResource_t<RMesh>(path);
}
