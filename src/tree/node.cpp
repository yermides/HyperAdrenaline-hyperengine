#include "node.hpp"
#include <util/macros.hpp>

Node::Node(bool const ignoreDrawInTraverse)
: m_ignoreDraw(ignoreDrawInTraverse)
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

    if(m_parent)
        m_parent->removeChild(this);

    INFOLOG("Starting to delete all childs from node "<< VAR(this));

    for(auto n : m_childs) {
        if(n) 
        {
            delete n;
            n = nullptr;
            INFOLOG("Deleted node "<< VAR(n));
        }
    }

    m_childs.clear();
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
    // TODO:: comprobar esto
    bool wantsUpdate = m_wantsUpdate;

    if(wantsUpdate) {
        m_transform = accumulatedTrans 
            * glm::translate(m_translation)
            * glm::rotate(glm::radians( m_rotation.x ), glm::vec3(1.0f,0.0f,0.0f))
            * glm::rotate(glm::radians( m_rotation.y ), glm::vec3(0.0f,1.0f,0.0f))
            * glm::rotate(glm::radians( m_rotation.z ), glm::vec3(0.0f,0.0f,1.0f))
            * glm::scale(m_scale);
        
        m_wantsUpdate = false;
    }
    
    if(m_entity && !m_ignoreDraw)
        m_entity->draw(m_transform);

    for(auto* child : m_childs) { 
        if(wantsUpdate)
            child->m_wantsUpdate = wantsUpdate;

        child->traverse(m_transform);
    }
}
