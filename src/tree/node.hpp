#pragma once
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "entities/entity.hpp"

namespace hyper {

struct Node
{
    using NodeList = std::vector<Node*>;
    
    explicit Node(bool const ignoreDrawInTraverse = false);
    ~Node();

    void                addChild(Node* node);
    void                removeChild(Node* node);

    inline glm::mat4 const    getUpdatedMatrixTransform()
    { 
        return m_transform * glm::translate(m_translation)*glm::rotate(glm::radians( m_rotation.x ), glm::vec3(1.0f,0.0f,0.0f))*glm::rotate(glm::radians( m_rotation.y ), glm::vec3(0.0f,1.0f,0.0f))*glm::rotate(glm::radians( m_rotation.z ), glm::vec3(0.0f,0.0f,1.0f))*glm::scale(m_scale); 
    }
    
    constexpr void                setMatrixTransform(glm::mat4 const& newTransform)		noexcept 
        { m_transform = newTransform; m_wantsUpdate = true;	}
    
    constexpr glm::mat4 const&    getMatrixTransform(void)								const noexcept 
        { return m_transform; 				}
    
    constexpr void                setEntity(Entity* newEntity)							noexcept 
        { m_entity = newEntity; m_wantsUpdate = true; }
    
    constexpr Entity*             getEntity(void)										const noexcept 
        { return m_entity; 					}
    
    constexpr void                setParent(Node* newNode)								noexcept 
        { m_parent = newNode; m_wantsUpdate = true;	}
    
    constexpr Node*               getParent(void)										const noexcept 
        { return m_parent; 					}
    
    constexpr void                setTranslation(glm::vec3 const& newTranslation)		noexcept 
        { m_translation = newTranslation; m_wantsUpdate = true; }
    
    constexpr glm::vec3 const&    getTranslation(void)									const noexcept 
        { return m_translation; 			}
    
    constexpr void                setRotation(glm::vec3 const& newRotation)				noexcept 
        { m_rotation = newRotation; m_wantsUpdate = true; }
    
    constexpr glm::vec3 const&    getRotation(void)										const noexcept 
        { return m_rotation; 				}
    
    constexpr void                setScale(glm::vec3 const& newScale)					noexcept 
        { m_scale = newScale; m_wantsUpdate = true;	}
    
    constexpr glm::vec3 const&    getScale(void)										const noexcept 
        { return m_scale;					}

    inline bool const             getChildNumber(void)									const noexcept 
        { return m_childs.size();					}

    inline int const              getNameID(void)                                       const noexcept
        { return m_name;                    }

    void                translate(glm::vec3 const& accumulation);
    void                rotate(glm::vec3 const& accumulation);
    void                scale(glm::vec3 const& accumulation);

    void                traverse(glm::mat4 const& accumulatedTrans);

    static void                deleteBranch(Node* node);
    static void                deleteBranchChilds(Node* node);
private:
    glm::mat4 m_transform {1.0f};
    NodeList m_childs;
    Node* m_parent { nullptr };
    Entity* m_entity { nullptr };

    glm::vec3 m_translation {0.0f};
    glm::vec3 m_rotation {0.0f};
    glm::vec3 m_scale {1.0f};

    bool m_wantsUpdate { true };

    // Variable usada para arreglar el recorrido del árbol 
    // no se renderizan las cámaras y luces durante el traverse
    // sino antes por control del engine
    bool const m_ignoreDraw { false };
    int m_name;
private:
    // Rootnode tendrá ID = 0 por hacer postincremento 
    inline static int nextNameID {0};
};

}
