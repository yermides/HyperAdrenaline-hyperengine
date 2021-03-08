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
            Args... args
        ,   Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        )
        {
            auto node       = createNode(parent, trans, rot, scale);
            auto camera     = new ECamera(args...);
            node->setEntity(camera);
            return node;
        }
    
    template <typename... Args>
    Node* createLight(
            Args... args
        ,   Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        )
        {
            auto node   = createNode(parent, trans, rot, scale);
            auto light  = new ELight(args...);
            node->setEntity(light);
            return node;
        }

    // template <typename... Args>
    // Node* createModel(
    //         Args... args
    //     ,   Node* const parent      = nullptr
    //     ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
    //     ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
    //     ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
    //     )
    //     {
    //         auto node   = createNode(parent, trans, rot, scale);
    //         auto model  = new EModel(args...);
    //         node->setEntity(model);
    //         return node;
    //     }


    // TODO:: tratar de hacer el mapeo de la variadic template al principio
    // Mirar https://www.bfilipek.com/2020/09/variadic-pack-first.html
    Node* createModel(
            std::string const& path
        ,   Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        )
        {
            auto node   = createNode(parent, trans, rot, scale);
            auto model  = new EModel(path);
            node->setEntity(model);
            return node;
        }
    
    void drawScene();

    constexpr GLFWwindow* getWindow(void) const noexcept
        { return m_window; }

private:
    Node* const     m_rootnode { new Node   };
    // No se necesita el resource manager por su naturaleza singleton

    // Atributos para mantenimiento de las cámaras, luces y viewports
    GLFWwindow*     m_window   { nullptr    };
};

