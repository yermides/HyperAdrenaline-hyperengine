#include "e_model.hpp"

EModel::EModel(/* args */)
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
EModel::draw(const glm::mat4& transform)
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
    m_rmesh->draw();

    // Unbind shader, if other shader is being used for other models
    glUseProgram(0);
}

void 
EModel::loadFromFile(const std::string& path)
{
    // m_rmesh = new RMesh();
	// m_rmesh->loadMesh(path);
    m_rmesh = ResourceManager::getResource_t<RMesh>(path);
}
