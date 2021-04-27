#include "node.hpp"
#include <util/macros.hpp>

namespace hyper {

Node::Node()
{
    INFOLOG("I am the node "<<VAR(this));
}

Node::~Node()
{
    // Borrado recursivo, TODO:: revisar, pues ha habido double frees
    INFOLOG("Deleting entity from node "<< VAR(this));

    if(m_entity)
    {
        delete m_entity;
        m_entity = nullptr;
    }

    INFOLOG("Removing parent from node "<< VAR(this));

    if(m_parent) {
        // necesario en algún momento llamar al removechild o hacer algo similar, pero de momento sirve
        // para deletear el rootnode
        m_parent = nullptr;
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
        INFOLOG("Soy dynamic");

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
                auto trans = util::glmVec3TobtVec3(m_translation);
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
                auto trans = util::glmVec3TobtVec3(this->getTranslation());
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
    if(!node) return;

    for(auto child : node->m_childs)
        deleteBranch(child);

    delete node;
    node = nullptr;
}

void
Node::deleteBranchChilds(Node* node)
{
    // TODO:: revisar, pero parece ir bien
    if(!node) return;

    for(auto n : node->m_childs)
    {
        if(n)
        {
            deleteBranch(n);
        }
    }

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

    auto forcedViewMatrix = glm::lookAt(
            getTranslation()
        ,   m_target
        ,   glm::vec3(0,1,0)
    );

    // auto position = glm::vec4(0,0,0,1) * forcedViewMatrix ;
    // auto pos = glm::vec3(position);
    // setTranslation(position);

    auto camera = static_cast<ECamera*>(m_entity);
    camera->setViewMatrix(forcedViewMatrix);

    
    // glm::quat orientation = glm::toQuat(forcedViewMatrix);
    glm::quat orientation = glm::conjugate(glm::toQuat(forcedViewMatrix));
    // auto newrot = glm::eulerAngleXYZ(orientation);
    // glm::vec3 newrot = glm::eulerAngles(orientation) * 3.14159f / 180.f;
    auto newrot = glm::eulerAngles(orientation);

    setRotation
    ( 
        // newrot
        glm::vec3
        (
                glm::degrees(newrot.x) 
            ,   glm::degrees(newrot.y)
            ,   glm::degrees(newrot.z) 
        )
    );

    // trasladar la rotación, test de descomponer la matriz
    // glm::mat4 transformation; // your transformation matrix.
    // glm::vec3 scale;
    // glm::quat rotation;
    // glm::vec3 translation;
    // glm::vec3 skew;
    // glm::vec4 perspective;
    // glm::decompose(transformation, scale, rotation, translation, skew, perspective);
    // rotation = glm::conjugate(rotation);
    // auto newrot = glm::eulerAngles(rotation);
    // newrot.x *=1.0f;
    // newrot.y *=1.0f;
    // newrot.z *=1.0f;
    // setRotation( 
    //     glm::degrees(
    //         glm::vec3{newrot.z, newrot.y, newrot.x}
    //         ) 
    //     );

    // setRotation( {69,69,69} );

    // setMatrixTransform(forcedViewMatrix);
}

}
