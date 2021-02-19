#include "node.hpp"

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
    // TODO::
}

void                
Node::rotate(const glm::vec3& accumulation) 
{
    // TODO::
}

void                
Node::scale(const glm::vec3& accumulation) 
{
    // TODO::
}

void 
Node::traverse(const glm::mat4& accumulatedTrans) 
{
    // TODO:: comprobar esto
    bool wantsUpdate = m_wantsUpdate;

    if(wantsUpdate) {
        m_transform = accumulatedTrans 
            * glm::translate(m_translation)
            * glm::rotate(0.0f, m_rotation) // TODO:: comprobar si el ángulo a pasar es realmente cero
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
