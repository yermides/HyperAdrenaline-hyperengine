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
#define default_trans_and_rot               default_trans, default_rot
#define default_rot_and_scale               default_rot, default_scale
#define default_matrix_params               default_trans, default_rot, default_scale
#define default_createnode_params           nullptr, default_matrix_params
#define engine_invalid_id                   -1

// Projection matrix
static glm::mat4 Projection 
	= glm::perspective(
		glm::radians(45.0f) // 45° Field of View
		, 16.0f / 9.0f      // 16:9 ratio
		, 0.1f              // display range : 0.1 unit <--
		, 100.0f            // --> 100 units
);

// View matrix
static glm::mat4 View  
	= glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);

// Model matrix, default model at 0,0,0
static glm::mat4 Model      = glm::mat4(1.0f);

// Our ModelViewProjection : multiplication of our 3 matrices
static glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

static glm::mat4 const test_projection = glm::lookAt(glm::vec3(4,3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
static glm::mat4 test_view = glm::mat4(1.0f);

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
            auto node       = createNode(parent, trans, rot, scale);
            auto camera     = new ECamera(args...);
            node->setEntity(camera);
            registerCamera(node);
            return node;
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
            auto node   = createNode(parent, trans, rot, scale);
            auto light  = new ELight(args...);
            node->setEntity(light);
            return node;
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
            auto node   = createNode(m_modelrootnode, trans, rot, scale);
            auto model  = new EModel(args...);
            node->setEntity(model);
            return node;
        }

    void clearScreen(void) const;

    void beginRender(void) const;

    void drawScene(void) const;

    void endRender(void) const;

    int registerCamera(Node* const camera);

    int registerLight(Node* const light);

    int registerViewport(int x, int y, int height, int width);

    void setActiveCamera(int const camID);

    void setActiveLight(int const lightID, bool const isActive);

    void setActiveViewport(int const viewportID);

    bool const isWindowActive(void) const noexcept;

    GLFWwindow* getWindow(void) const noexcept;

    bool const isKeyPressed(int const keycode) const noexcept;

private:
    inline static int nextCameraID      {0};
    inline static int nextLightID       {0};
    inline static int nextViewportID    {0};

    struct Viewport {
        int x, y, height, width;
    };

    Node* const     m_rootnode      { new Node   };
    Node* const     m_modelrootnode      { createNode()   };
    // No se necesita el resource manager por su naturaleza singleton

    // Atributos para mantenimiento de las cámaras, luces y viewports
    GLFWwindow*     m_window   { nullptr    };
    std::vector<Node*> m_cameras;
    std::vector<Node*> m_lights;
    std::vector<Viewport> m_viewports;
    int m_active_camera  {engine_invalid_id} 
    ,   m_active_viewport{engine_invalid_id};
    std::vector<bool> m_active_lights;
};
