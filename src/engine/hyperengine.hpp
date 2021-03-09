#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <managers/resource_manager.hpp>
#include <tree/node.hpp>
#include <entities/e_camera.hpp>
#include <entities/e_light.hpp>
#include <entities/e_model.hpp>

#define default_trans                       {0.0f,0.0f,0.0f}
#define default_rot                         {0.0f,0.0f,0.0f}
#define default_scale                       {1.0f,1.0f,1.0f}
#define default_matrix_params               default_trans, default_rot, default_scale
#define default_createnode_params           nullptr, default_matrix_params

struct HyperEngine
{
    explicit HyperEngine();
    ~HyperEngine();

    void initialize(void);

    Node* createNode(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        );

    template <typename... Args>
    Node* createCamera(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args... args
        )
        {
            // auto node       = createNode(parent, trans, rot, scale);
            // auto camera     = new ECamera(args...);
            // node->setEntity(camera);
            // return node;
            return create<ECamera>(parent, trans, rot, scale, args...);
        }
    
    template <typename... Args>
    Node* createLight(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args... args
        )
        {
            // auto node   = createNode(parent, trans, rot, scale);
            // auto light  = new ELight(args...);
            // node->setEntity(light);
            // return node;
            return create<ELight>(parent, trans, rot, scale, args...);
        }

    template <typename... Args>
    Node* createModel(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args... args
        )
        {
            // auto node   = createNode(parent, trans, rot, scale);
            // auto model  = new EModel(args...);
            // node->setEntity(model);
            // return node;
            return create<EModel>(parent, trans, rot, scale, args...);
        }
    
    template <typename Entity_t, typename... Args>
    Node* create(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args... args
        )
        {
            auto node       = createNode(parent, trans, rot, scale);
            auto entity     = new Entity_t(args...);
            node->setEntity(entity);
            return node;
        }
    
    void drawScene();

    constexpr GLFWwindow* getWindow(void) const noexcept
        { return m_window; }

private:
    // Leer el remark del final sobre los nodos
    Node* const     m_rootnode { new Node   };
    // No se necesita el resource manager por su naturaleza singleton

    // Atributos para mantenimiento de las cámaras, luces y viewports
    GLFWwindow*     m_window   { nullptr    };
};

// Él propone que se creen los nodos cámara y luz antes de los modelos, pero eso supone que 
// no se puedan meter más de una (porque se añadirían a la derecha de los hijos del rootnode)
// , por lo que una solucion sería que el rootnode tenga tres hijos
// y que cada uno administre las luces, cámara y modelos
// y las inserciones de nuevos nodos se hagan en estos que actúan de manejadores
// 
// Declarados en orden, el esquema sería:
// 
//               ---------- lightmanagernode (hijos...)
//               ^
// rootnode ----- > ------- cameramanagernode (hijos...)
//               v
//               ---------- modelmanagernode (hijos...)
