#pragma once
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "entities/entity.hpp"

struct Node
{
    using NodeList = std::vector<Node*>;
    
    Node();
    ~Node();

    void                addChild(Node* node);
    void                removeChild(Node* node);
    
    constexpr void                setMatrixTransform(const glm::mat4& newTransform)		noexcept 
        { m_transform = newTransform; m_wantsUpdate = true;	}
    
    constexpr const glm::mat4&    getMatrixTransform(void)								const noexcept 
        { return m_transform; 				}
    
    constexpr void                setEntity(Entity* newEntity)							noexcept 
        { m_entity = newEntity; m_wantsUpdate = true; }
    
    constexpr Entity*             getEntity(void)										const noexcept 
        { return m_entity; 					}
    
    constexpr void                setParent(Node* newNode)								noexcept 
        { m_parent = newNode; m_wantsUpdate = true;	}
    
    constexpr Node*               getParent(void)										const noexcept 
        { return m_parent; 					}
    
    constexpr void                setTranslation(const glm::vec3& newTranslation)		noexcept 
        { m_translation = newTranslation; m_wantsUpdate = true; }
    
    constexpr const glm::vec3&    getTranslation(void)									const noexcept 
        { return m_translation; 			}
    
    constexpr void                setRotation(const glm::vec3& newRotation)				noexcept 
        { m_rotation = newRotation; m_wantsUpdate = true; }
    
    constexpr const glm::vec3&    getRotation(void)										const noexcept 
        { return m_rotation; 				}
    
    constexpr void                setScale(const glm::vec3& newScale)					noexcept 
        { m_scale = newScale; m_wantsUpdate = true;	}
    
    constexpr const glm::vec3&    getScale(void)										const noexcept 
        { return m_scale;					}

    void                translate(const glm::vec3& accumulation);
    void                rotate(const glm::vec3& accumulation);
    void                scale(const glm::vec3& accumulation);

    void                traverse(const glm::mat4& accumulatedTrans);

private:
    glm::mat4 m_transform {1.0f};
    Entity* m_entity { nullptr };
    NodeList m_childs;
    Node* m_parent { nullptr };

    glm::vec3 m_translation {0.0f};
    glm::vec3 m_rotation {0.0f};
    glm::vec3 m_scale {1.0f};

    bool m_wantsUpdate { true };
};
