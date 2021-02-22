#include "node.hpp"
#include <util/macros.hpp>

Node::Node(/* args */)
{
}

Node::~Node()
{
}

void                
Node::addChild(Node* node) 
{
    m_childs.push_back(node);
}

void                
Node::removeChild(Node* node) 
{
    // TODO:: revisar implementación, no lo borra, solo lo remueve del arbol
    if( m_childs.empty() ) 
        return;

    auto it = std::find_if(m_childs.begin(), m_childs.end(), [&](Node* n) {
        return n == node;
    });

    if( it == m_childs.end() )
        return;

    m_childs.erase(it);
}

void                
Node::translate(const glm::vec3& accumulation) 
{
    m_translation += accumulation;
    m_wantsUpdate = true;
}

void                
Node::rotate(const glm::vec3& accumulation) 
{
    m_rotation += accumulation;
    m_wantsUpdate = true;
}

void                
Node::scale(const glm::vec3& accumulation) 
{
    m_scale += accumulation;
    m_wantsUpdate = true;
}

void 
Node::traverse(const glm::mat4& accumulatedTrans) 
{
    // TODO:: comprobar esto
    bool wantsUpdate = m_wantsUpdate;

    if(wantsUpdate) {
        m_transform = accumulatedTrans 
            * glm::translate(m_translation)
            * glm::rotate(m_rotation.x, glm::vec3(1,0,0))
            * glm::rotate(m_rotation.y, glm::vec3(0,1,0))
            * glm::rotate(m_rotation.z, glm::vec3(0,0,1))
            * glm::scale(m_scale);
        
        m_wantsUpdate = false;
    }
    
    m_entity->draw(m_transform);

    for(auto* child : m_childs) { 
        if(wantsUpdate)
            child->m_wantsUpdate = wantsUpdate;

        child->traverse(m_transform);
    }
}
