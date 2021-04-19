#pragma once
// C++
#include <vector>
#include <algorithm>
// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
// Bullet physics
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
// HyperEngine
#include <util/physics.hpp>
#include "entities/entity.hpp"

#define default_first_node_id 0

namespace hyper {

struct Node
{
    // Alias para la lista de nodos
    using NodeList = std::vector<Node*>;
    using NodeID   = int;
    
    explicit Node();
    ~Node();

    void                addChild(Node* node);
    void                removeChild(Node* node);

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

    constexpr PhysicProperties* getPhysicProperties(void)
        { return m_physicProperties;        }

    constexpr void setPhysicProperties(PhysicProperties* properties)
        { m_physicProperties = properties;  }

    void                translate(glm::vec3 const& accumulation);
    void                rotate(glm::vec3 const& accumulation);
    void                scale(glm::vec3 const& accumulation);

    void                traverse(glm::mat4 const& accumulatedTrans);

    static void         deleteBranch(Node* node);
    static void         deleteBranchChilds(Node* node);
private:
    // Transformación gráfica del objeto en el mundo y opciones de dibujado
    glm::mat4 m_transform   { 1.0f };   // Matriz resultado de operar con los valores de debajo
    glm::vec3 m_translation { 0.0f };   // Vector de tres floats que informa de la posición
    glm::vec3 m_rotation    { 0.0f };   // Vector de tres floats que informa de la rotación
    glm::vec3 m_scale       { 1.0f };   // Vector de tres floats que informa del escalado 
    bool m_wantsUpdate      { true };   // Booleano que informa de si hay que actualizar la matriz en la iteración

    // Datos importantes relacionados del nodo
    NodeList m_childs;                                      // Lista de hijos del nodo
    Node* m_parent                          { nullptr };    // Padre del nodo
    Entity* m_entity                        { nullptr };    // Entidad con datos importantes del nodo
    PhysicProperties *m_physicProperties    { nullptr };    // Propiedades físicas si tiene
    NodeID m_name                           { default_first_node_id };  // Para compatibilidad, identificador del nodo
private:
    inline static NodeID nextNameID         { default_first_node_id };  // Autoincrementable identificador
};

}
