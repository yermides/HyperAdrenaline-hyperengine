#include "node.hpp"

Node::Node(/* args */)
{
}

Node::~Node()
{
}

void 
Node::traverse(const glm::mat4& accumulatedTrans) 
{
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
