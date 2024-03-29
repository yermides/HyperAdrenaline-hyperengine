#include "node.hpp"
#include <util/macros.hpp>

namespace hyen {

Node::Node()
{
    // INFOLOG("I am the node "<< VAR(this));
}

Node::~Node()
{
    // Cambiado el 6/5/2021, por si hay que volver a lo anterior

    // Borrado recursivo, TODO:: revisar, pues ha habido double frees
    // INFOLOG("Deleting entity from node "<< VAR(this));

    if(m_entity)
    {
        delete m_entity;
        m_entity = nullptr;
    }

    // INFOLOG("Removing parent from node "<< VAR(this));

    if(m_parent) {
        // necesario en algún momento llamar al removechild o hacer algo similar, pero de momento sirve
        // para deletear el rootnode
        m_parent->removeChild(this);
    }
}

void                
Node::addChild(Node* node) 
{
    if(node->m_parent)
        node->m_parent->removeChild(node);

    node->m_parent = this;
    m_childs.push_back(node);
}

void                
Node::removeChild(Node* node) 
{
    // No lo borra, solo lo remueve del arbol
    if(!node->m_parent) return;
    if(m_childs.empty()) return;

    auto it = std::find_if(m_childs.begin(), m_childs.end(), [&](Node* n) {
        return n == node;
    });

    if( it == m_childs.end() )
        return;

    (*it)->m_parent = nullptr;
    m_childs.erase(it);
}

void                
Node::translate(glm::vec3 const& accumulation) 
{
    m_translation += accumulation;
    m_wantsUpdate = true;
}

void                
Node::rotate(glm::vec3 const& accumulation) 
{
    m_rotation += accumulation;
    m_wantsUpdate = true;
}

void                
Node::scale(glm::vec3 const& accumulation) 
{
    m_scale += accumulation;
    m_wantsUpdate = true;
}

void 
Node::traverse(glm::mat4 const& accumulatedTrans) 
{
    // Falta considerar el escalado en el traspaso de situación del objeto gráfico/físico

    if
    ( 
        m_physicProperties 
    &&  m_physicProperties->m_type == PhysicProperties::PhysicDatatype::RIGID_BODY
    &&  !m_physicProperties->m_data.body->isKinematicObject() 
    &&  !m_physicProperties->m_data.body->isStaticObject() 
    )
    {
        // INFOLOG("Soy dynamic");

        auto body = m_physicProperties->m_data.body;
        auto motionState = m_physicProperties->m_motionState;

        // Aplicar transformaciones al objeto de bullet (obviando el escalado)
        btTransform transform;
        motionState->getWorldTransform(transform);

        auto trans = util::btVec3ToGlmVec3(transform.getOrigin());
        this->setTranslation(trans);

        auto btquatrot = transform.getRotation();
        auto eulerrot = glm::vec3(0,0,0);
        btquatrot.getEulerZYX(eulerrot.z, eulerrot.y, eulerrot.x);
        // btquatrot.getEulerZYX(eulerrot.y, eulerrot.x, eulerrot.z);
        eulerrot.x = glm::degrees(eulerrot.x);
        eulerrot.y = glm::degrees(eulerrot.y);
        eulerrot.z = glm::degrees(eulerrot.z);
        this->setRotation(eulerrot);
    }

    // TODO:: comprobar esto
    bool wantsUpdate = m_wantsUpdate;

    // if(m_isCamera) goto cameraskip;

    // Cambiar la comprobación final por !isStatic para que los static no puedan ser modificados, los dynamichay que controlar que no se modifiquen desde fuera
    if( wantsUpdate /* && ( !m_physicProperties || ( m_physicProperties && m_physicProperties->m_body->isKinematicObject() ) ) */ ) {
        m_transform = accumulatedTrans 
            * glm::translate(m_translation)
            * glm::rotate(glm::radians( m_rotation.x ), glm::vec3(1.0f,0.0f,0.0f))
            * glm::rotate(glm::radians( m_rotation.y ), glm::vec3(0.0f,1.0f,0.0f))
            * glm::rotate(glm::radians( m_rotation.z ), glm::vec3(0.0f,0.0f,1.0f))
            * glm::scale(m_scale);
        
        m_wantsUpdate = false;

        if(m_physicProperties)
        {
            if(m_physicProperties->m_type == PhysicProperties::PhysicDatatype::COLLISION_OBJECT)
            {
                auto collisionObject = m_physicProperties->m_data.collObj;
                btTransform transform;

                // Translación
                auto trans = util::glmVec3TobtVec3(m_translation + m_physicOffset);
                transform.setOrigin(trans);

                // Rotación
                btQuaternion btquatx( {1.0,0.0,0.0}, glm::radians( m_rotation.x ) );
                btQuaternion btquaty( {0.0,1.0,0.0}, glm::radians( m_rotation.y ) );
                btQuaternion btquatz( {0.0,0.0,1.0}, glm::radians( m_rotation.z ) );
                btQuaternion btquat = btquatx * btquaty * btquatz;
                transform.setRotation(btquat);

                collisionObject->setWorldTransform(transform);
            }
            else if(m_physicProperties->m_type == PhysicProperties::PhysicDatatype::RIGID_BODY && m_physicProperties->m_data.body->isKinematicObject())
            {
                auto body = m_physicProperties->m_data.body;
                auto motionState = m_physicProperties->m_motionState;
                btTransform transform;

                // TODO:: realmente no es la translación, sino m_transform, la columna de la translación (para tener en cuenta la translación heredada) y lo mismo para la rotación
                // Recordar que se puede hacer cast a mat3 desde mat4 y directamente crear un btTransform((casteada)mat3, transfrom->getOrigin());

                body->getMotionState()->getWorldTransform(transform);

                // Translación
                auto trans = util::glmVec3TobtVec3(getTranslation() + m_physicOffset);
                transform.setOrigin( trans );

                // Rotación
                btQuaternion btquatx( {1.0,0.0,0.0}, glm::radians( m_rotation.x ) );
                btQuaternion btquaty( {0.0,1.0,0.0}, glm::radians( m_rotation.y ) );
                btQuaternion btquatz( {0.0,0.0,1.0}, glm::radians( m_rotation.z ) );
                btQuaternion btquat = btquatx * btquaty * btquatz;
                transform.setRotation(btquat);

                // Aplicar transformaciones al objeto de bullet (obviando el escalado)
                body->getMotionState()->setWorldTransform(transform);
            }
            else if(m_physicProperties->m_type == PhysicProperties::PhysicDatatype::KINEMATIC_CHARACTER)
            {
                btPairCachingGhostObject* object { m_physicProperties->m_data.ghostObj };
                btTransform& transform { object->getWorldTransform() };
                glm::vec3 glmtrans { util::btVec3ToGlmVec3(transform.getOrigin()) };
                setTranslation(glmtrans - m_physicOffset);
            }
        }
    }

cameraskip:
    if(m_entity && m_isVisible)
        m_entity->draw(m_transform);

    for(auto* child : m_childs) { 
        if(wantsUpdate)
            child->m_wantsUpdate = wantsUpdate;

        child->traverse(m_transform);
    }
}

void 
Node::deleteBranch(Node* node) 
{
    // Cambiado el 6/5/2021, por si hay que volver a lo anterior
    if(!node) return;
    deleteBranchChilds(node);

    delete node;
    node = nullptr;
}

void
Node::deleteBranchChilds(Node* node)
{
    // Cambiado el 6/5/2021, por si hay que volver a lo anterior
    if(!node) return;

    while(!node->m_childs.empty())
        deleteBranch(node->m_childs[0]);

    node->m_childs.clear();
}

glm::vec3 const&    
Node::getCameraTarget()
{
    return m_target;
}

void                
Node::setCameraTarget(glm::vec3 const& target)
{
    m_target = target;
    auto camera = getEntityAsCamera();
    camera->setTarget(m_target);

    auto forcedViewMatrix = glm::lookAt(
            getTranslation()
        ,   m_target
        ,   glm::vec3(0,1,0) // Hardcoded up
    );

    // auto position = glm::vec4(0,0,0,1) * forcedViewMatrix ;
    // auto pos = glm::vec3(position);
    // setTranslation(position);

    camera->setViewMatrix(forcedViewMatrix);

    // Meter en la rotación de la cámara su inversa de su rotación para los hijos que tenga
    glm::quat orientation = glm::quat_cast( glm::mat3(forcedViewMatrix) ); 
    auto newrot = glm::degrees( glm::eulerAngles(orientation) ) * (-1.0f);

    setRotation( newrot );

}

}
