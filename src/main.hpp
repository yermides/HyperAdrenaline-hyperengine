#pragma once
#include <iostream>
#include <memory>
#include <chrono>
#include <ratio>
#define __cplusplus 201703L
#include <filesystem>
#include <dirent.h>
#include <util/macros.hpp>
#include <engine/hyperengine.hpp>
// #include <entities/e_light.hpp>
#include <entities/e_model.hpp>
#include <entities/e_animatedmodel.hpp>
#include <tree/node.hpp>
#include <util/functions.hpp>

// Esto es para una prueba, cube_test
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
// Esto es para una prueba, cube_test

// Esto es para una prueba, loading_textures_soil_test
#include <SOIL/SOIL.h>

// Esto es para una prueba, imgui_testing
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

// Esto es para una prueba, print matrix
#include <glm/gtx/string_cast.hpp>

// Flags para testeo
enum Flags_t {
        f0 = 1 << 0
    ,   f1 = 1 << 1
    ,   f2 = 1 << 2
    ,   f3 = 1 << 3
    ,   f4 = 1 << 4
    ,   f5 = 1 << 5
    ,   f6 = 1 << 6
    ,   f7 = 1 << 7
};

// Tests anteriores

// void test_multiple_lights(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_multiple_lights");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {5,0,0}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    


//     // [[maybe_unused]] 
//     // hyen::Node* lightnode = engine->createLight(
//     //     default_createnode_params
//     // );

//     hyen::Node* lightnode = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode3 = engine->createLight(
//             nullptr
//         ,   glm::vec3(2.0f,-3.5f,0.0f)
//         ,   default_rot_and_scale   
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.02f, 0.02f, 0.2f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.009f}, .quadratic{0.00032f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode2 = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Directional
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
//         ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { -0.2f, -1.0f, -0.3f }
//     );


//     [[maybe_unused]] 
//     hyen::Node* missile_launcher   = engine->createModel(nullptr, {2,-1,0}, default_rot_and_scale, "assets/missile-launcher.obj");


//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-0.2,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/planes/semicube.obj"
//     );

//     [[maybe_unused]] 
//     hyen::Node* cubito_rosa = engine->createModel(default_createnode_params, "assets/cubito_rosa.obj");


//     plane->setNameID(1);

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         // Input
//         if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//             camnode->rotate({0,3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//             camnode->rotate({0,-3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//             camnode->rotate({3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//             camnode->rotate({-3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//             camnode->translate({-.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//             camnode->translate({.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//             camnode->translate({0,0,-.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//             camnode->translate({0,0,.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//             camnode->translate({0,.3f,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//             camnode->translate({0,-.3f,0});

//         // auto lightentity = lightnode->getEntityAsLight();
//         // INFOLOG("attenuation constant: " << VAR(lightentity->getAttenuation().constant))

//         // Cámara siempre apunta a 0,0,0
//         camnode->setCameraTarget({0,0,0});
//         // Update de las físicas
//         engine->updatePhysics();
//     }
// }

// // Se llama test de sliding speed pero es comprobar el punto de choque: su ángulo con la superficie y su distancia en altura
// void test_physics_sliding_speed(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_physics_sliding_speed");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {-5,2,0}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode3 = engine->createLight(
//             nullptr
//         ,   glm::vec3(2.0f,-3.5f,0.0f)
//         ,   default_rot_and_scale   
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.02f, 0.02f, 0.2f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.009f}, .quadratic{0.00032f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode2 = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Directional
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
//         ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { -0.2f, -1.0f, -0.3f }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-0.2,0}
//         ,   {0,0,0}
//         ,   default_scale
//         // ,   "assets/planes/semicube.obj"
//         ,   "assets/planes/character-test.obj"
//         // ,   "assets/pruebacolisiones.obj"
//     );

//     [[maybe_unused]] 
//     hyen::Node* cubito_rosa = engine->createModel(
//             nullptr
//         ,   {0,1,0}
//         ,   default_rot
//         ,   {0.2f,0.2f,0.2f}
//         ,   "assets/cubito_rosa.obj"
//     );

//     engine->enableDebugDraw();
//     plane->setNameID(1);
//     cubito_rosa->setNameID(2);

//     engine->createPhysicPropertiesTriangleMeshShape(
//             plane
//         ,   0
//         ,   0b00000010
//         ,   0b00000001
//     );

//     // engine->createPhysicPropertiesCollisionObject(
//     //         cubito_rosa
//     //     ,   new btBoxShape({0.2f,0.2f,0.2f})
//     //     ,   0b00000001
//     //     ,   0b00000010
//     // );
    
//     // Datos para Liam
//     engine->createPhysicPropertiesKinematicCharacterController(
//             cubito_rosa
//         ,   0.5f
//         ,   1.8f
//         ,   2.0f
//         ,   0.9f
//     );

//     // engine->createPhysicPropertiesKinematicCharacterController(
//     //         cubito_rosa
//     //     ,   0.5f
//     //     ,   0.0f
//     //     ,   2.0f
//     //     ,   0.5f
//     // );

//     auto& controller = cubito_rosa->getPhysicProperties()->charCon;
//     controller->setLinearVelocity({0.00001f, 0.0f, 0.00001f});

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         // Input controls
//         if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//             camnode->rotate({0,3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//             camnode->rotate({0,-3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//             camnode->rotate({3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//             camnode->rotate({-3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//             camnode->translate({-.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//             camnode->translate({.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//             camnode->translate({0,0,-.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//             camnode->translate({0,0,.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//             camnode->translate({0,.3f,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//             camnode->translate({0,-.3f,0});

//         // Input debug
//         if(engine->getKeySinglePress(GLFW_KEY_0))
//         {
//             engine->deletePhysicProperties(cubito_rosa);
//             engine->deletePhysicProperties(plane);
//         }

//         if(engine->getKeySinglePress(GLFW_KEY_1))
//         {
//             engine->createPhysicPropertiesKinematicCharacterController(
//                     cubito_rosa
//                 ,   2.3f
//                 ,   1.0f
//             );

//             engine->createPhysicPropertiesTriangleMeshShape(
//                     plane
//                 ,   0
//                 ,   0b00000010
//                 ,   0b00000001
//             );
//         }

//         // Check collision
//         if(engine->getKeySinglePress(GLFW_KEY_ENTER))
//         {
//             CollisionPairResult res;
//             bool collides = engine->getCollisionBetweenNodes(plane, cubito_rosa, res);

//             INFOLOG( "collides: " << VAR(collides) )

//             if(collides)
//                 INFOLOG( "id's: " << VAR(res.IDs.first) << VAR(res.IDs.second) )
//         }

//         // {
//         //     CollisionPairResult res;
//         //     bool collides = engine->getCollisionBetweenNodes(plane, cubito_rosa, res);

//         //     if(collides)
//         //     {
//         //         cubito_rosa->translate({0,0.01f,0});
//         //     }
//         // }

//         // Move the cube
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_8))
//         //     cubito_rosa->translate({0.0f,0.0f,0.2f});
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_2))
//         //     cubito_rosa->translate({0.0f,0.0f,-0.2f});
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_4))
//         //     cubito_rosa->translate({0.2f,0.0f,0.0f});
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_6))
//         //     cubito_rosa->translate({-0.2f,0.0f,0.0f});
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_7))
//         //     cubito_rosa->translate({0.0f,-0.2f,0.0f});
//         // if(engine->getKeySinglePress(GLFW_KEY_KP_9))
//         //     cubito_rosa->translate({0.0f,0.2f,0.0f});


//         if(engine->getKeyContinuousPress(GLFW_KEY_2))
//         {
//             engine->deleteAllWorldPhysics();
//         }

//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_1))
//         {
//             if(controller->onGround())
//             {
//                 controller->jump({0,5,0});
//             }
//         }

//         controller->setWalkDirection({0,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_8))
//             controller->setWalkDirection(btVector3(0,0,1).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_2))
//             controller->setWalkDirection(btVector3(0,0,-1).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_4))
//             controller->setWalkDirection(btVector3(1,0,0).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_6))
//             controller->setWalkDirection(btVector3(-1,0,0).normalized() / 10);

//         // updating stuff
//         // cubito_rosa->translate({0,-0.01f,0});

//         // camnode->setCameraTarget({100,2,0});
//         // camnode->setTranslation(cubito_rosa->getTranslation() - glm::vec3{10,0,0});

//         camnode->setCameraTarget({0,0,0});
//         engine->updatePhysics();
//     }
// }

// void test_physics_world_importer(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_physics_world_importer");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {-5,2,0}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode3 = engine->createLight(
//             nullptr
//         ,   glm::vec3(2.0f,-3.5f,0.0f)
//         ,   default_rot_and_scale   
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.02f, 0.02f, 0.2f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.009f}, .quadratic{0.00032f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* lightnode2 = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Directional
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
//         ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { -0.2f, -1.0f, -0.3f }
//     );

    

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-0.2,0}
//         ,   {0,0,0}
//         ,   default_scale
//         // ,   "assets/planes/semicube.obj"
//         ,   "assets/planes/character-test.obj"
//         // ,   "assets/mapa1joined.obj"
//         // ,   "assets/pruebacolisiones.obj"
//         // ,   "assets/planes/mapa3.obj"
//         // ,   "assets/planes/halllway.obj"
//     );

//     [[maybe_unused]] 
//     hyen::Node* cubito_rosa = engine->createModel(
//             nullptr
//         ,   {0,1,0}
//         ,   default_rot
//         ,   {0.2f,0.2f,0.2f}
//         ,   "assets/cubito_rosa.obj"
//     );

//     engine->enableDebugDraw();
//     plane->setNameID(1);
//     cubito_rosa->setNameID(2);

//     engine->createPhysicPropertiesTriangleMeshShape(
//             plane
//         ,   0
//         ,   8
//         ,   -1
//     );

//     // engine->createPhysicPropertiesFromArchive(
//     //         plane
//     //     // ,   "assets/physics/fisica.bullet"
//     //     ,   "assets/physics/mapa1.bullet"
//     //     // ,   "assets/physics/entrenamiento.bullet"
//     //     // ,   "assets/physics/mapa1.dae"
//     // );

//     // Datos para Liam
//     engine->createPhysicPropertiesKinematicCharacterController(
//             cubito_rosa
//         ,   0.5f
//         ,   1.8f
//         ,   2.0f
//         ,   0.9f
//         ,   2
//     );

//     auto& controller = cubito_rosa->getPhysicProperties()->charCon;
//     controller->setLinearVelocity({0.00001f, 0.0f, 0.00001f});

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         // Input controls
//         if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//             camnode->rotate({0,3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//             camnode->rotate({0,-3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//             camnode->rotate({3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//             camnode->rotate({-3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//             camnode->translate({-.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//             camnode->translate({.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//             camnode->translate({0,0,-.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//             camnode->translate({0,0,.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//             camnode->translate({0,.3f,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//             camnode->translate({0,-.3f,0});

//         // Input debug
//         if(engine->getKeySinglePress(GLFW_KEY_0))
//         {
//             engine->deletePhysicProperties(cubito_rosa);
//             engine->deletePhysicProperties(plane);
//         }

//         if(engine->getKeyContinuousPress(GLFW_KEY_2))
//         {
//             engine->deleteAllWorldPhysics();
//         }

//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_1))
//         {
//             if(controller->onGround())
//             {
//                 controller->jump({0,5,0});
//             }
//         }

//         controller->setWalkDirection({0,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_8))
//             controller->setWalkDirection(btVector3(0,0,1).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_2))
//             controller->setWalkDirection(btVector3(0,0,-1).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_4))
//             controller->setWalkDirection(btVector3(1,0,0).normalized() / 10);
//         if(engine->getKeyContinuousPress(GLFW_KEY_KP_6))
//             controller->setWalkDirection(btVector3(-1,0,0).normalized() / 10);

//         // updating stuff
//         camnode->setCameraTarget({0,0,0});
//         engine->updatePhysics();
//     }
// }

// void test_animated_model(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_animated_model");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,5}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto   

//     [[maybe_unused]] 
//     hyen::Node* lightnode = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     hyen::Node* robot = engine->createAnimatedModel(
//             default_createnode_params
//         ,   "assets/animations/robot/"
//         ,   0.025                                       // Intervalo entre frames
//     );

//     auto animation = robot->getEntityAsAnimatedModel();
//     auto current = animation->getCurrentAnimation();

//     animation->getAnimation("walking")->setLooped(true);

//     // Si peta es casi al 100% que sea porque no hay aún un createAnimatedModel en hyperengine.cpp, construir el ejemplo bien de una vez pls
//     // auto amodel { hyen::EAnimatedModel("assets/animations/robot/") };

//     const double fpsLimit = 1.0 / 60.0;
//     double lastUpdateTime = 0;  // number of seconds since the last loop
//     double lastFrameTime = 0;   // number of seconds since the last frame

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         double now = glfwGetTime();
//         double deltaTime = now - lastUpdateTime;
//         animation->updateAnimation(deltaTime);

//         // anim->update(deltaTime);
//         // robot->translate({0,0,1.0 * deltaTime});
//         // INFOLOG("Framerate: " << 1.0 / deltaTime) // Not accurrate

//         // This if-statement only executes once every 60th of a second
//         if ((now - lastFrameTime) >= fpsLimit)
//         {
//             // fpsLimit is being treated as a fixed timestep
//             // Input controls
//             if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//                 camnode->rotate({0,3,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//                 camnode->rotate({0,-3,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//                 camnode->rotate({3,0,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//                 camnode->rotate({-3,0,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//                 camnode->translate({-5.0 * fpsLimit,0,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//                 camnode->translate({.3,0,0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//                 camnode->translate({0,0,-.3f});
//             if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//                 camnode->translate({0,0,.3f});
//             if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//                 camnode->translate({0,5.0f * fpsLimit, 0});
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//                 camnode->translate({0,-5.0f * fpsLimit, 0});

//             if(engine->getKeySinglePress(GLFW_KEY_1))
//                 current->setFrameIndex(0, true);
//             if(engine->getKeySinglePress(GLFW_KEY_2))
//                 current->setFrameIndex(1);
//             if(engine->getKeySinglePress(GLFW_KEY_3))
//                 current->setFrameIndex(2);
//             if(engine->getKeySinglePress(GLFW_KEY_4))
//                 current->setFrameIndex(3);
//             if(engine->getKeySinglePress(GLFW_KEY_5))
//                 current->setFrameIndex(4);

//             if(engine->getKeySinglePress(GLFW_KEY_6))
//                 animation->setAnimation("walking");
//             if(engine->getKeySinglePress(GLFW_KEY_7))
//                 animation->setAnimation("death");
//             if(engine->getKeySinglePress(GLFW_KEY_8))
//                 animation->setAnimation("death22222222");
//             // draw your frame here

//             // Render
//             engine->beginRender();
//             engine->drawScene();
//             engine->endRender();

//             // only set lastFrameTime when you actually draw something
//             lastFrameTime = now;
//         }


//         // updating stuff
//         camnode->setCameraTarget({0,1,0});
//         engine->updatePhysics( /* deltaseconds.count() */ );

//         lastUpdateTime = now;
//     }
// }

// void test_map2_error(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_map2_error");
//     engine->setWindowIcon("assets/logo.jpg");
//     // engine->enableDebugDraw();

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,5}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     // auto camentity { camnode->getEntityAsCamera() };
//     // camentity->setParallel(45.0f, 16.0f/9.0f, 0.1f, 100.f);
//     // camentity->setParallel();

//     [[maybe_unused]] 
//     hyen::Node* lightnode = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* lightnode3 = engine->createLight(
//             nullptr
//         ,   glm::vec3(0.0f,4.0f,2.0f)
//         ,   default_rot_and_scale   
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.2f, 0.2f, 0.2f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00f}, .quadratic{0.000f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* lightnode2 = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Directional
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
//         ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { -0.0f, -0.0f, -1.0f }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,0,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/mapa/mapa2joined.obj"
//     );
//     // engine->createPhysicPropertiesTriangleMeshShape(
//     //     plane
//     // );

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         // Input controls
//         if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//             camnode->rotate({0,3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//             camnode->rotate({0,-3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//             camnode->rotate({3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//             camnode->rotate({-3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//             camnode->translate({-.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//             camnode->translate({.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//             camnode->translate({0,0,-.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//             camnode->translate({0,0,.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//             camnode->translate({0,.3f,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//             camnode->translate({0,-.3f,0});

//         // updating stuff
//         camnode->setCameraTarget({0,0,0});
//         engine->updatePhysics();
//     }
// }

// void test_normal_mapping(void) {
//     std::unique_ptr<hyen::HyperEngine> engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_normal_mapping");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,-4}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode2 = engine->createLight(
//             default_createnode_params
//         ,   hyen::LightType::Directional
//         ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
//         ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { -0.2f, -1.0f, -0.3f }
//     );

//     hyen::Node* lightnode = engine->createLight(
//             nullptr
//         ,   {0,1,-1}
//         ,   default_rot_and_scale
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-1,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/planes/semicube.obj"
//     );

//     [[maybe_unused]] 
//     hyen::Node* machine = engine->createModel(
//             nullptr
//         ,   {0,0,0}
//         ,   {0,0,0}
//         ,   {0.2,0.2,0.2}
//         ,   "assets/objects/AmmoMachine.obj"
//     );

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         // Input controls
//         if(engine->getKeyContinuousPress(GLFW_KEY_A))       
//             camnode->rotate({0,3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_D))       
//             camnode->rotate({0,-3,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_W))       
//             camnode->rotate({3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_S))       
//             camnode->rotate({-3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
//             camnode->translate({-.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
//             camnode->translate({.3,0,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
//             camnode->translate({0,0,-.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
//             camnode->translate({0,0,.3f});
//         if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
//             camnode->translate({0,.3f,0});
//         if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
//             camnode->translate({0,-.3f,0});

//         // updating stuff
//         camnode->setCameraTarget(machine->getTranslation());
//         engine->updatePhysics();
//     }
// }

// void test_deltatime(void){
//     auto engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_deltatime");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,-4}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode = engine->createLight(
//             nullptr
//         ,   {0,0,0}
//         ,   default_rot_and_scale
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-1,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/planes/semicube.obj"
//     );

//     const double fpsLimit = 1.0 / 60.0;
//     double lastUpdateTime = 0;  // number of seconds since the last loop
//     double lastFrameTime = 0;   // number of seconds since the last frame

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         double now = glfwGetTime();
//         double deltaTime = now - lastUpdateTime;
//         float dt { static_cast<float>(deltaTime) }; 

//         // This if-statement only executes once every 60th of a second
//         if ((now - lastFrameTime) >= fpsLimit)
//         {
//             // Input controls, updatin positions
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  5.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -5.0f,  0.0f} * dt);

//             // updating stuff
//             camnode->setCameraTarget({0,0,0});
//             engine->updatePhysics(deltaTime);
            
//             lastFrameTime = now;
//         }

//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         lastUpdateTime = now;
//     }
// }

// void test_particle_system(void) {
//     auto engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_particle_system");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,-4}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode = engine->createLight(
//             nullptr
//         ,   {0,0,0}
//         ,   default_rot_and_scale
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,-1,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/planes/semicube.obj"
//     );

//     // engine->createParticleSystem();
//     engine->createParticleGenerator();

//     const double fpsLimit = 1.0 / 60.0;
//     double lastUpdateTime = 0;  // number of seconds since the last loop
//     double lastFrameTime = 0;   // number of seconds since the last frame

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         double now = glfwGetTime();
//         double deltaTime = now - lastUpdateTime;
//         float dt { static_cast<float>(deltaTime) }; 

//         // This if-statement only executes once every 60th of a second
//         if ((now - lastFrameTime) >= fpsLimit)
//         {
//             // Input controls, updatin positions
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  5.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -5.0f,  0.0f} * dt);

//             // updating stuff
//             camnode->setCameraTarget({0,0,0});
//             engine->updatePhysics(deltaTime);
//             // engine->updateParticleGenerator(dt);
//             // engine->updateParticleSystem(dt);

//             lastFrameTime = now;
//         }

//         // Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         lastUpdateTime = now;
//     }
// }

// void test_particle_system_2(void) {
//     auto engine = std::make_unique<hyen::HyperEngine>(true);
//     engine->setWindowTitle("test_particle_system_2");
//     engine->setWindowIcon("assets/logo.jpg");

//     [[maybe_unused]] 
//     hyen::Node* camnode = engine->createCamera(
//             nullptr
//         ,   {0,0,-4}
//         ,   default_rot_and_scale
//     ); // tendrá la proyección por defecto    

//     hyen::Node* lightnode = engine->createLight(
//             nullptr
//         ,   {0,0,0}
//         ,   default_rot_and_scale
//         ,   hyen::LightType::Point
//         ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//         ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
//         ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//         ,   hyen::LightDirection   { 0,0,0 }
//     );

//     [[maybe_unused]] 
//     hyen::Node* plane = engine->createModel(
//             nullptr
//         ,   {0,0,0}
//         ,   {0,0,0}
//         ,   default_scale
//         ,   "assets/planes/semicube.obj"
//     );

//     [[maybe_unused]]
//     hyen::Node* missile_launcher = engine->createModel(
//             camnode
//         ,   camnode->getTranslation() + glm::vec3{1.5,-.8,0}
//         ,   {0,180,0}
//         ,   default_scale
//         ,   "assets/missile-launcher.obj"
//     );

//     hyen::ParticleGenerator::CInfo cInfo
//     {
//         .maxParticles{10000}
//     ,   .texturePath{"assets/particles/particle.DDS"}
//     ,   .origin{0,0,0}
//     // ,   .gravity{0.0f, -9.81f, 0.0f}
//     ,   .gravity{0.0f, 0.0f, 0.0f}
//     ,   .mainDir{0.0f, 10.0f, 0.0f}
//     ,   .particlesPerSecond{100.0f}
//     ,   .spreadFactor{1.5f}
//     ,   .lifeSpan{5.0f}
//     ,   .minParticleSize{0.15f}
//     ,   .maxParticleSize{0.5f}
//     ,   .shapeRadius{3.f}
//     // ,   .funcColor{&hyen::PGF::generateParticleColorsStandard}
//     ,   .funcColor{&hyen::PGF::generateParticleColorsRandomly}
//     // ,   .funcColor{&hyen::PGF::generateParticleColorsAndTransparencyRandomly}
//     ,   .funcSize{&hyen::PGF::generateParticleSizeBetween}
//     ,   .funcRandomdir{&hyen::PGF::generateRandomDirectionSoftInfluence}
//     ,   .funcPos{&hyen::PGF::generatePositionCameraTarget}
//     ,   .funcMaindir{&hyen::PGF::generateMainDirectionCameraTarget}
//     };

//     // hyen::ParticleGenerator::CInfo cInfo
//     // {
//     //     .maxParticles{10000}
//     // ,   .texturePath{"assets/particles/particle.DDS"}
//     // ,   .origin{0,0,0}
//     // ,   .gravity{0.0f, 0.0f, 0.0f}
//     // ,   .mainDir{0.0f, 0.0f, 0.0f}
//     // ,   .particlesPerSecond{100.0f}
//     // ,   .spreadFactor{1.5f}
//     // ,   .lifeSpan{5.0f}
//     // ,   .minParticleSize{0.15f}
//     // ,   .maxParticleSize{0.5f}
//     // ,   .shapeRadius{5.f}
//     // ,   .funcColor{&hyen::PGF::generateParticleColorsRandomly}
//     // ,   .funcSize{&hyen::PGF::generateParticleSizeBetween}
//     // ,   .funcRandomdir{&hyen::PGF::generateRandomDirectionStandard}
//     // ,   .funcPos{&hyen::PGF::generateRandomPositionBoxShape}
//     // ,   .funcMaindir{&hyen::PGF::generateMainDirectionStandard}
//     // };

//     static std::vector<hyen::ParticleGenerator*> generators;
//     generators.reserve(50);

//     // for (size_t i = 0; i < 50; i++)
//     // {
//     //     generators.push_back(engine->createParticleGenerator(cInfo));
//     // }
    

//     auto generator = engine->createParticleGenerator(cInfo);
//     generator->setActive(true, true, 3.0f);
//     // auto generator2 = engine->createParticleGenerator(cInfo);

    

//     const double fpsLimit = 1.0 / 60.0;
//     double lastUpdateTime = 0;  // number of seconds since the last loop
//     double lastFrameTime = 0;   // number of seconds since the last frame

//     while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//     {
//         double now = glfwGetTime();
//         double deltaTime = now - lastUpdateTime;
//         float dt { static_cast<float>(deltaTime) }; 

//         // This if-statement only executes once every 60th of a second
//         if ((now - lastFrameTime) >= fpsLimit)
//         {
//             // Input controls, updatin positions
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  5.0f,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  5.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  5.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -5.0f,  0.0f} * dt);
            
//             auto const& pos { camnode->getTranslation() + glm::vec3{0,0,10} };

//             generator->setOrigin({0,0,0});

//             if(engine->getKeyContinuousPress(GLFW_KEY_KP_1))
//             { 
//                 // generator->setGravity({0,0,0});
//                 generator->setFuncPos(&hyen::PGF::generatePositionStatic);
//                 generator->setFuncMaindir(&hyen::PGF::generateMainDirectionStandard);
//                 generator->setOrigin(pos);
//             }

//             if(engine->getKeyContinuousPress(GLFW_KEY_KP_2))
//             {
//                 generator->setActive(true);
//             }

//             if(engine->getKeyContinuousPress(GLFW_KEY_KP_3))
//             {
//                 generator->setActive(false);
//             }

//             // updating stuff
//             camnode->setCameraTarget({0,0,0});
//             engine->updatePhysics(deltaTime);

//             generator->setMatrices(camnode);
//             // generator2->setMatrices(camnode);

//             for (auto gen : generators)
//             {
//                 gen->setMatrices(camnode);
//                 gen->update(dt);
//             }

//             generator->update(dt);
//             // generator2->update(dt);

//             lastFrameTime = now;
//         }

//         // Render
//         engine->beginRender();
//         engine->drawScene();

//         for (auto gen : generators)
//         {
//             gen->render();
//         }

//         generator->render();
//         // generator2->render();

//         engine->endRender();

//         lastUpdateTime = now;
//     }
// }