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

// This matrix belongs to the ECamera
// Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
static glm::mat4 Projection 
	= glm::perspective(
		glm::radians(45.0f)
		, 16.0f / 9.0f
		, 0.1f
		, 100.0f
);

// This matrix belongs to the Node that contains ECamera (and is inverted)
// Camera matrix
static glm::mat4 View  
	= glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);

// This matrix belongs to the Node that contains the EMesh
// Model matrix : an identity matrix (model will be at the origin)
static glm::mat4 Model      = glm::mat4(1.0f);

// Our ModelViewProjection : multiplication of our 3 matrices
static glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

struct HyperEngine
{
    explicit HyperEngine(bool const init = false);
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

    void clearScreen(void) const;

    void beginRender(void) const;

    void drawScene(void) const;

    void endRender(void) const;


    constexpr bool const isWindowActive(void) const noexcept
        { 
            if(!m_window || glfwWindowShouldClose(m_window))
                return false;

            return true;
            // return m_window ? glfwWindowShouldClose(m_window) : false; 
        }

    constexpr GLFWwindow* getWindow(void) const noexcept
        { return m_window; }

    constexpr bool const isKeyPressed(int const keycode) const noexcept
        { 
            if(!isWindowActive())
                return false;

            return glfwGetKey(m_window, keycode) == GLFW_PRESS; 
        }

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


