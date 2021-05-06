#pragma once
// C++
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>
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
// Bullet physics
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>

// HyperEngine
#include <tree/node.hpp>
#include <resources/r_shader.hpp>
#include <entities/e_camera.hpp>
#include <entities/e_light.hpp>
#include <entities/e_model.hpp>
#include <entities/e_skybox.hpp>
#include <util/debugdrawer.hpp>

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

// #define SHADER_DEFAULT_PATH                 "src/shaders/model-loading-m-v-p" 
// #define SHADER_DEFAULT_PATH                 "src/shaders/materials" 
// #define SHADER_DEFAULT_PATH                 "src/shaders/materials-and-lights" 
#define SHADER_DEFAULT_PATH                 "src/shaders/multiple-lights" 

#define SHADER_SKYBOX_PATH                  "src/shaders/skybox"
#define SHADER_DEBUGDRAWER_PATH             "src/shaders/debugdrawer"

template<typename T1, typename T2>
using Hashmap = std::unordered_map<T1, T2>;

namespace hyper {

namespace gui = ImGui;

// Abstracción del nombre de la ruta de los shaders
enum class OpenGLShader {
        SHADER_DEFAULT
    ,   SHADER_SKYBOX
    ,   SHADER_DEBUGDRAWER
};

// Declaración de estructuras
// struct RayResult    { btRigidBody* pBody; btVector3 hitPoint; Node* node;           };

struct RayResult        { btCollisionObject* pObj; btVector3 hitPoint; Node* node;      };
struct Viewport         { int x, y, width, height;                                      };
struct MouseWheelStatus { float offsetX {0.0f}, offsetY {0.0f};                         };
struct PhysicContactResult {};

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
        ,   Args&&... args
        )
        {
            auto node       = createNode(parent, trans, rot, scale);
            auto camera     = new ECamera(args...);

            // Test, poner directamente el shader a usar
            camera->setShader(m_shaders[OpenGLShader::SHADER_DEFAULT]);

            node->setEntity(camera);
            registerCamera(node);
            node->setIsCamera(true);
            return node;
        }
    
    template <typename... Args>
    Node* createLight(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args&&... args
        )
        {
            auto node   = createNode(parent, trans, rot, scale);
            auto light  = new ELight(args...);

            // Test, poner directamente el shader a usar
            light->setShader(m_shaders[OpenGLShader::SHADER_DEFAULT]);

            node->setEntity(light);
            auto id = registerLight(node);
            light->setID(id);
            return node;
        }

    template <typename... Args>
    Node* createModel(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args&&... args
        )
        {
            auto node   = createNode(parent, trans, rot, scale);
            auto model  = new EModel(args...);

            // Test, poner directamente el shader a usar
            model->setShader(m_shaders[OpenGLShader::SHADER_DEFAULT]);

            node->setEntity(model);
            return node;
        }

    template <typename... Args>
    Node* createSkybox(
            Node* const parent      = nullptr
        ,   glm::vec3 const& trans  = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& rot    = {0.0f,0.0f,0.0f}
        ,   glm::vec3 const& scale  = {1.0f,1.0f,1.0f} 
        ,   Args&&... args
        )
        {
            auto node       = createNode(parent, trans, rot, scale);
            auto skybox     = new ESkybox(args...);

            // Usando un shader propio, aún en testeo
            skybox->setShader(m_shaders[OpenGLShader::SHADER_SKYBOX]);

            node->setEntity(skybox);
            registerSkybox(node);
            return node;
        }

    // Relacionado con la estructura

    void clearTree(void);

    // Relacionados con el renderizado

    void clearScreen(void) const;

    void beginRender(void);

    void drawScene(void);

    void drawExampleWindowGUI(void);    // TODO:: borrar, es solo un ejemplo

    void endRender(void) const;

    // Registro de elementos entidad

    int registerCamera(Node* const camera);

    int registerLight(Node* const light);

    int registerViewport(int x, int y, int height, int width);

    int registerSkybox(Node* const skybox);

    void setActiveCamera(int const camID);

    void setActiveLight(int const lightID, bool const isActive);

    void setActiveViewport(int const viewportID);

    void setActiveSkybox(Node* const node);

    bool isSkyboxActive(void) const noexcept;

    void deleteSkybox(void);

    // Útiles

    // Ventana

    bool const isWindowActive(void) const noexcept;

    GLFWwindow* getWindow(void) const noexcept;

    // Input de teclado

    bool const getKeySinglePress(int const key) noexcept;

    bool const getKeyContinuousPress(int const key) noexcept;

    bool const getKeyKeyboardPress(int const key) noexcept;

    bool const getKeyRelease(int const key) noexcept;

    bool const getMouseKeySinglePress(int const key) noexcept;

    bool const getMouseKeyContinuousPress(int const key) noexcept;

    bool const getMouseKeyKeyboardPress(int const key) noexcept;

    bool const getMouseKeyRelease(int const key) noexcept;

    MouseWheelStatus const& getMouseWheelStatus(void);

    glm::dvec2 getMousePositionAbsolute(void) const noexcept;

    void setMousePositionAbsolute(float x, float y);

    glm::dvec2 getMousePositionRelative(void) const noexcept;

    void setMousePositionRelative(float x, float y);

    void setMousePositionToCenter(void);

    // Estas dos de abajo están en pruebas

    void setCursorVisibility(bool const value);

    void setCursorPosition(double const x = 0.5, double const y = 0.5);

    // Comprobación de si quedan nodos en el árbol

    bool const isTreeEmpty(void);

    // Cosas cute

    glm::ivec2 getWindowSize(void);

    glm::ivec2 getWindowSizeCenter(void);

    void setWindowSize(int const x, int const y);

    void setWindowTitle(std::string const& name);

    void setWindowIcon(std::string const& path, int const width = 32, int const height = 32);

    void setWindowClearColor(float const r, float const g, float const b, float const a);

    void setWindowActive(bool const value); 

    // Optimizaciones de renderizado

    void enableZBuffer(int const method = GL_LESS);

    void disableZBuffer(void);

    void enableCulling(int const method = GL_BACK, int const faceselection = GL_CW);

    void disableCulling(void);

    // Físicas

    void updatePhysics(float const deltatime = 1.0f / 60.0f);

    // Las crea como objeto estático, TODO:: hacer una función para objetos estáticos, cinemáticos y dinámicos

    // Función llamada por todas las que crean rigidbody, sea static, kinematic o dynamic
    void createPhysicPropertiesRigidBody(
            Node* const node
        ,   btCollisionShape* pShape
        ,   float mass
        ,   int collisionGroupFlags = 0
        ,   int collisionMaskFlags  = 0
    );

    void createPhysicPropertiesCollisionObject(
            Node* const node
        ,   btCollisionShape* pShape
        ,   int collisionGroupFlags = 2
        ,   int collisionMaskFlags  = -3
    );

    void createPhysicPropertiesStaticBody(
            Node* const node
        ,   btCollisionShape* pShape
        ,   int collisionGroupFlags = 0
        ,   int collisionMaskFlags  = 0
    );

    void createPhysicPropertiesKinematicBody(
            Node* const node
        ,   btCollisionShape* pShape
        ,   int collisionGroupFlags = 0
        ,   int collisionMaskFlags  = 0
    );

    void createPhysicPropertiesDynamicBody(
            Node* const node
        ,   btCollisionShape* pShape
        ,   float mass = 1
        ,   int collisionGroupFlags = 0
        ,   int collisionMaskFlags  = 0
    );

    // Las colisiones que usarán los mapas, WARNING, solo funciona con meshes totalmente trianguladas, also, debería ser estatic o cinematic (no dynamic)
    void createPhysicPropertiesTriangleMeshShape(
            Node* const node
        ,   float mass = 0
        ,   int collisionGroupFlags = 0
        ,   int collisionMaskFlags  = 0
    );

    void createPhysicPropertiesKinematicCharacterController(
            Node* const node
        ,   float capsuleRadius = 0.5f
        ,   float capsuleHeight = 1.0f
        ,   float jumpHeight = 2.0f
        ,   float stepHeight = 0.05f
        ,   int collisionMaskFlags = 0
    );

    void createPhysicPropertiesFromArchive(Node* const node, std::string const& filepath);

    // Solo comprueba el AABB, seguramente ni se use
    bool getAABBCollisionBetweenNodes(Node* const nodeA, Node* const nodeB);

    bool getCollisionBetweenNodes(Node* const nodeA, Node* const nodeB, CollisionPairResult& collPairResult);

    void deletePhysicProperties(Node* const node);

    void deleteAllWorldPhysics(void);

    // ************************************************

    // Funciones patateras, mejoradas arriba
    void createRigidbody(Node * const node);

    // Realmente no es convex hull, es la geometría pura (todo es por el testing)
    void createRigidBodyConvexHull(Node * const node);

    // habría que parametrizar, equis dé
    void createRigidBodyDynamic(Node * const node = nullptr);

    // Las colisiones que usarán los mapas
    void createTriangleMeshShape(Node * const node);

    // ************************************************


    // Funciones necesarias para el raycast (2/2)
    bool throwRaycast(
            const btVector3 &startPosition
        ,   const btVector3 &direction
        ,   RayResult &output
        ,   int collisionGroupMask = 2147483647
        ,   int collisionFilterMask = 2147483647
    );

    bool throwRaycastAllHits(
            const btVector3 &startPosition
        ,   const btVector3 &direction
        ,   std::vector<RayResult>& output
        ,   int collisionGroupMask = 2147483647
        ,   int collisionFilterMask = 2147483647
    );

    bool checkRaycastCollisionWithNode(const btVector3 &startPosition, const btVector3 &direction);

    void drawDebugPhysics(glm::mat4 const& view, glm::mat4 const& projection);

    void enableDebugDraw(void);

    void disableDebugDraw(void);

    DebugDrawer* const getDebugDrawer(void);

    void setDebugDrawer(DebugDrawer* debugDrawer);

    void resetKeyStates(void);
    void resetMouseKeyStates(void);
    void resetMouseWheelStatus(void);
private:
    // Inicializadores, solo se llaman en el constructor
    void initializeGraphics(void);
    void initializePhysics(void);

    // Funciones auxiliares
    void setKeyState(int const key, int const action);
    void setMouseKeyState(int const key, int const action);
    void setMouseWheelStatus(float const offsetX, float const offsetY);

    // No se necesita el resource manager por su naturaleza singleton

    Node* const     m_rootnode      { new Node   };

    // Administrador de shaders
    Hashmap<OpenGLShader, RShader*> m_shaders;

    // Administración del input de teclado
    Hashmap<int, int> m_keystates;
    Hashmap<int, int> m_mousekeystates;
    MouseWheelStatus m_mouseWheelStatus;

    // Atributos para mantenimiento de las cámaras, luces y viewports  (y ahora skybox)
    GLFWwindow*             m_window   { nullptr };
    std::vector<Node*>      m_cameras;
    std::vector<Viewport>   m_viewports;
    int m_active_camera  {engine_invalid_id} 
    ,   m_active_viewport{engine_invalid_id};

    Node* m_skybox  { nullptr };

    // std::vector<LightData> m_lights;
    std::vector<Node*> m_lights;
    std::vector<bool> m_active_lights;

    // Físicas (bullet)
    btDiscreteDynamicsWorld* m_world    { nullptr };
    DebugDrawer* m_debugDrawer          { nullptr };
    bool m_useDebugDrawer               { false   };
    std::vector<CollisionPairResult> m_collisionPairs;
    std::vector<btKinematicCharacterController*> m_characterControllers; // Necesario para el cleanup de las físicas de la escena
    btBulletWorldImporter* m_importer   { nullptr };

    // std::vector<std::pair<Node::NodeID, Node::NodeID>> m_collisionPairs;

    // imgui
    ImGuiIO* m_io;

    // ID's para los elementos
    inline static int nextCameraID      {0};
    inline static int nextLightID       {0};
    inline static int nextViewportID    {0};

    // Aún en duda de si usar esta estructura
    // struct LightData {
    //     Node* m_node;
    //     bool m_active;
    // };
};

}
