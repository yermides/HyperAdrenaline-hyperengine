#include "e_camera.hpp"

namespace hyper {

// ECamera::ECamera()
// : Entity{}
// {
// }

ECamera::ECamera(glm::mat4 const& projection)
: Entity{}, m_projection(projection)
{
}

ECamera::~ECamera()
{
}

void 
ECamera::draw(glm::mat4 const& transform) 
{
    if(!this->getProgramID())
    {
        ERRLOG("No tengo un shader con el que renderizar la cámara");
        return;
    }

    m_view = glm::inverse(transform);

    // INFOLOG("Se encuentra la propiedad projection: " << VAR(glGetUniformLocation(m_programID, "projection")));

    // // Enviar la matriz proyección
    // glUniformMatrix4fv(
    //         glGetUniformLocation(m_programID, "projection")
    //     ,   1
    //     ,   GL_FALSE
    //     ,   &m_projection[0][0]
    // );

    // INFOLOG("Se encuentra la propiedad view: " << VAR(glGetUniformLocation(m_programID, "view")));

    // // Enviar la inversa de la matriz vista
    // glUniformMatrix4fv(
    //         glGetUniformLocation(m_programID, "view")
    //     ,   1
    //     ,   GL_FALSE
    //     ,   &m_view[0][0]
    // );
}

}