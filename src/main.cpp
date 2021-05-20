/*
    This file exists only for testing
    it'll be removed once the engine
    is compiled into an .a file
*/

// Este test es para copiar y pegar en nuevas funciones

// void test_generic(void)
// {
//     using namespace hyen;

//     auto engine = std::make_unique<HyperEngine>(true);
//     engine->setWindowTitle("test_generic");
//     engine->setWindowIcon("assets/logo.jpg");

//     auto camnode = engine->createCamera(
//         nullptr
//     ,   {0,0,-5}
//     ,   default_rot_and_scale
//     );

//     auto lightnode = engine->createLight(
//         nullptr
//     ,   {0,0,0}
//     ,   default_rot_and_scale
//     ,   hyen::LightType::Point
//     ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
//     ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
//     ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
//     ,   hyen::LightDirection   { 0,0,0 }
//     );

//     auto missile_launcher = engine->createModel(
//         camnode
//     ,   camnode->getTranslation() + glm::vec3{1.5,-0.8,0}
//     ,   {0,180,0}
//     ,   default_scale
//     ,   "assets/missile-launcher.obj"
//     );

//     constexpr double const fpsLimit = 1.0 / 60.0;
//     double lastUpdateTime = 0.0;  // number of seconds since the last loop
//     double lastFrameTime = 0.0;   // number of seconds since the last frame

//     while(engine->isWindowActive())
//     {
//         double now = glfwGetTime();
//         double deltaTime = now - lastUpdateTime;
//         float dt { static_cast<float>(deltaTime) }; 

//         This if-statement only executes once every 60th of a second
//         if ((now - lastFrameTime) >= fpsLimit)
//         {
//             if(engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
//                 engine->setWindowActive(false);

//             Input controls, updatin positions
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -camSpeed,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  camSpeed,  0.0f,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -camSpeed} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  camSpeed} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  camSpeed,  0.0f} * dt);
//             if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -camSpeed,  0.0f} * dt);


//             updating stuff
//             camnode->setCameraTarget({0,0,0});
//             engine->updatePhysics(deltaTime);

//             lastFrameTime = now;
//         }

//         Render
//         engine->beginRender();
//         engine->drawScene();
//         engine->endRender();

//         lastUpdateTime = now;
//     }

// }

#include <main.hpp>

void test_frustum(void)
{
    using namespace hyen;

    auto engine = std::make_unique<HyperEngine>(true);
    engine->setWindowTitle("test_frustum");
    engine->setWindowIcon("assets/engine/logo.jpg");

    auto camnode = engine->createCamera(
        nullptr
    ,   {0,1,-5}
    ,   default_rot_and_scale
    );

    auto lightnode = engine->createLight(
        nullptr
    ,   {0,0,0}
    ,   default_rot_and_scale
    ,   hyen::LightType::Point
    ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
    ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.0}, .quadratic{0.0} }
    ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
    ,   hyen::LightDirection   { 0,0,0 }
    );

    auto map = engine->createModel(
        default_createnode_params
    ,   "assets/game/maps/theHangarJoined.obj"
    );

    engine->createPhysicPropertiesTriangleMeshShape(
        map
    );

    std::vector<Node*> enemies;
    enemies.reserve(100);

    for (size_t i = 0; i < 100; i++)
    {
        auto m = engine->createModel(
            nullptr
        ,   {-69,0,10*i}
        ,   default_rot_and_scale
        ,   "assets/game/characters/melee/noanim.obj"
        );

        // engine->createPhysicPropertiesCollisionObject(m, new btBoxShape({1,1,1}), -1, -1);
        enemies.push_back(m);
    }
    
    auto missile_launcher = engine->createModel(
        camnode
    ,   camnode->getTranslation() + glm::vec3{1.5,-0.8,0}
    ,   {0,180,0}
    ,   default_scale
    ,   "assets/engine/missile-launcher.obj"
    );

    auto skyboxnode = engine->createSkybox(
        default_createnode_params
    ,   SkyboxNamelist 
        { 
            "assets/engine/skybox/space/lightblue/top.png"
        ,   "assets/engine/skybox/space/lightblue/bot.png"
        ,   "assets/engine/skybox/space/lightblue/left.png"
        ,   "assets/engine/skybox/space/lightblue/right.png"
        ,   "assets/engine/skybox/space/lightblue/front.png"
        ,   "assets/engine/skybox/space/lightblue/back.png" 
        }
    );

    // std::vector<float> tests;
    // auto& vertices {map->getEntityAsModel()->getVertexPositions()};
    // tests.reserve(vertices.size() * 2 * 3); 
    // tests.insert(tests.end(), vertices.begin(), vertices.end());
    // tests.insert(tests.end(), vertices.begin(), vertices.end());
    // tests.insert(tests.end(), vertices.begin(), vertices.end());
    // tests.insert(tests.end(), vertices.begin(), vertices.end());
    // tests.insert(tests.end(), vertices.begin(), vertices.end());
    // tests.insert(tests.end(), vertices.begin(), vertices.end());

    constexpr double const fpsLimit = 1.0 / 60.0;
    constexpr float const baseSpeed = 15.0f;
    double lastUpdateTime = 0.0;  // number of seconds since the last loop
    double lastFrameTime = 0.0;   // number of seconds since the last frame
    float camSpeed = baseSpeed;

    while(engine->isWindowActive())
    {
        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;
        float dt { static_cast<float>(deltaTime) }; 

        // This if-statement only executes once every 60th of a second
        if ((now - lastFrameTime) >= fpsLimit)
        {
            // tests.clear();
            // INFOLOG(tests.size());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            // tests.insert(tests.end(), vertices.begin(), vertices.end());
            for(auto enemy : enemies)
            {
                // Test, no dibujar si estamos lejos (para mejorar performance)
                auto enemyTrans {enemy->getTranslation()};
                auto cameraTrans {camnode->getTranslation()};
                auto le = enemyTrans - cameraTrans;
                auto length = glm::length(le);
                // INFOLOG("diff vector" << VAR(le.x) << VAR(le.y) << VAR(le.z))
                // INFOLOG("length " << length )
                // INFOLOG("length from enemy = " << le)

                if(length > 50)
                {
                    enemy->setVisible(false);
                }
                else{
                    enemy->setVisible(true);
                }

            }


            if(engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
                engine->setWindowActive(false);

            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_SHIFT))
                camSpeed = baseSpeed * 10;
            else
                camSpeed = baseSpeed;

            // Input controls, updatin positions
            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{  camSpeed,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  -camSpeed,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, camSpeed} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  -camSpeed} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  camSpeed,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -camSpeed,  0.0f} * dt);

            if(engine->getKeyContinuousPress(GLFW_KEY_1))           
                engine->enableDebugDraw();
            if(engine->getKeyContinuousPress(GLFW_KEY_2))           
                engine->disableDebugDraw();

            if(engine->getKeyContinuousPress(GLFW_KEY_3))           
                map->setVisible(true);

            if(engine->getKeyContinuousPress(GLFW_KEY_4))           
                map->setVisible(false);

            // updating stuff
            camnode->setCameraTarget({0,1,1000});
            engine->updatePhysics(deltaTime);

            lastFrameTime = now;

            // INFOLOG(vertices.size());
            // INFOLOG(tests.size() * 4); // Máximo de bytes alojándose en el debugdrawer si fuera con un vector de líneas (6 megas)
        }

        // Render
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        lastUpdateTime = now;
    }

}

int main(void)
{
    test_frustum();
}
