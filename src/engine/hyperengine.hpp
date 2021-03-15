#pragma once
// C++
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <map>
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <managers/resource_manager.hpp>
// ImGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
// HyperEngine
#include <tree/node.hpp>
#include <resources/r_shader.hpp>
#include <entities/e_camera.hpp>
#include <entities/e_light.hpp>
#include <entities/e_model.hpp>

// Create node params
#define default_trans                       {0.0f,0.0f,0.0f}
#define default_rot                         {0.0f,0.0f,0.0f}
#define default_scale                       {1.0f,1.0f,1.0f}
#define default_trans_and_rot               default_trans, default_rot
#define default_rot_and_scale               default_rot, default_scale
#define default_matrix_params               default_trans, default_rot, default_scale
#define default_createnode_params           nullptr, default_matrix_params
// Cameras, lights, etc
#define engine_invalid_id                   -1
// Shader paths
#define SHADER_DEFAULT_PATH                 "src/shaders/model-loading-m-v-p" 

namespace hyper {

namespace gui = ImGui;

// Abstracción del nombre de la ruta de los shaders
enum class OpenGLShader {
    SHADER_DEFAULT
};

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

            // Test, poner directamente el shader a usar
            camera->setProgramID(m_shaders[OpenGLShader::SHADER_DEFAULT]->getProgramID());

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

            // Test, poner directamente el shader a usar
            light->setProgramID(m_shaders[OpenGLShader::SHADER_DEFAULT]->getProgramID());

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
            auto node   = createNode(parent, trans, rot, scale);
            auto model  = new EModel(args...);

            // Test, poner directamente el shader a usar
            model->setProgramID(m_shaders[OpenGLShader::SHADER_DEFAULT]->getProgramID());

            node->setEntity(model);
            return node;
        }

    void clearScreen(void) const;

    void beginRender(void);

    void drawScene(void) const;

    void drawExampleWindowGUI(void);    // TODO:: borrar, es solo un ejemplo

    void endRender(void) const;

    int registerCamera(Node* const camera);

    int registerLight(Node* const light);

    int registerViewport(int x, int y, int height, int width);

    void setActiveCamera(int const camID);

    void setActiveLight(int const lightID, bool const isActive);

    void setActiveViewport(int const viewportID);

    bool const isWindowActive(void) const noexcept;

    GLFWwindow* getWindow(void) const noexcept;

    void setKeyState(int const key, int const action);

    bool const getKeySinglePress(int const key) noexcept;
    bool const getKeyContinuousPress(int const key) noexcept;
    bool const getKeyKeyboardPress(int const key) noexcept;
    bool const getKeyRelease(int const key) noexcept;

private:
    void resetKeyStates(void);

    struct Viewport {
        int x, y, height, width;
    };

    inline static int nextCameraID      {0};
    inline static int nextLightID       {0};
    inline static int nextViewportID    {0};


    Node* const     m_rootnode      { new Node   };
    // No se necesita el resource manager por su naturaleza singleton

    // Administrador de shaders
    std::unordered_map<OpenGLShader, RShader*> m_shaders;
    // Administración del input de teclado
    std::map<int, int> m_keystates;
    // Atributos para mantenimiento de las cámaras, luces y viewports
    GLFWwindow*     m_window   { nullptr    };
    std::vector<Node*> m_cameras;
    std::vector<Node*> m_lights;
    std::vector<Viewport> m_viewports;
    int m_active_camera  {engine_invalid_id} 
    ,   m_active_viewport{engine_invalid_id};
    std::vector<bool> m_active_lights;

    // imgui
    ImGuiIO* m_io;
};

}


// // Projection matrix
// static glm::mat4 Projection 
// 	= glm::perspective(
// 		glm::radians(45.0f) // 45° Field of View
// 		, 16.0f / 9.0f      // 16:9 ratio
// 		, 0.1f              // display range : 0.1 unit <--
// 		, 100.0f            // --> 100 units
// );

// // View matrix
// static glm::mat4 View  
// 	= glm::lookAt(
// 		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
// 		glm::vec3(0,0,0), // and looks at the origin
// 		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
// );

// // Model matrix, default model at 0,0,0
// static glm::mat4 Model      = glm::mat4(1.0f);

// // Our ModelViewProjection : multiplication of our 3 matrices
// static glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

// static glm::mat4 const test_projection = glm::lookAt(glm::vec3(4,3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
// static glm::mat4 test_view = glm::mat4(1.0f);