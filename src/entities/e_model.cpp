#include "e_model.hpp"

EModel::EModel(/* args */)
{
}

EModel::~EModel()
{
    // TODO:: Resource Manager should delete this, but there is no Resource Manager so delete it here
    delete m_rmesh;
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
}

void 
EModel::loadFromFile(const std::string& path)
{
    m_rmesh = new RMesh();
	m_rmesh->loadMesh("cubo precioso");
}
