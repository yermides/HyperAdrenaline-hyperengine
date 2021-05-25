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
    // última prueba, 
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

    map->setNameID(1);

    engine->createPhysicPropertiesRigidBody(
        map
        ,   new btBoxShape({50,1,20})
        ,   0
        ,   -1
        ,   -1
    );

    // engine->createPhysicPropertiesTriangleMeshShape(
    //     map
    // );

    // auto ghostObject = engine->createGhostObject(new btBoxShape({60, 10, 30}));

    auto sensornode = engine->createNode(default_createnode_params);
    engine->createPhysicPropertiesGhostObject(sensornode, new btBoxShape({60, 10, 30}));
    map->setNameID(2);

    std::vector<Node*> enemies;
    enemies.reserve(100);

    for (size_t i = 0; i < 100; i++)
    {
        auto m = engine->createModel(
            nullptr
        ,   {-3,0,10*i}
        // ,   {-69,0,10*i}
        ,   default_rot_and_scale
        ,   "assets/game/characters/melee/noanim.obj"
        );

        m->setNameID(i+3);

        engine->createPhysicPropertiesCollisionObject(m, new btBoxShape({1,1,1}), -1, -1);
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
            INFOLOG("collision for the ghostObj? " << VAR(engine->getAABBCollisionBetweenNodes(sensornode, map)) )

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

                bool inside = engine->getAABBCollisionBetweenNodes(sensornode, enemy);

                if(inside)
                {
                    enemy->setVisible(true);
                }
                else{
                    enemy->setVisible(false);
                }

                // if(length > 50)
                // {
                //     enemy->setVisible(false);
                // }
                // else{
                //     enemy->setVisible(true);
                // }

            }

            // engine->getGhostObjectCollisions(ghostObject);

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

            sensornode->translate(glm::vec3{  0.0f,  0.0f, 3.0f} * dt);

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

void test_normal_mapping(void)
{
    using namespace hyen;

    auto engine = std::make_unique<HyperEngine>(true);
    engine->setWindowTitle("test_normal_mapping");
    engine->setWindowIcon("assets/engine/logo.jpg");

    auto camnode = engine->createCamera(
        nullptr
    ,   {0,0,-5}
    ,   default_rot_and_scale
    );

    auto lightnode = engine->createLight(
        nullptr
    ,   {0,0,0}
    ,   default_rot_and_scale
    ,   hyen::LightType::Point
    ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
    ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
    ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
    ,   hyen::LightDirection   { 0,0,0 }
    );

    auto missile_launcher = engine->createModel(
        camnode
    ,   camnode->getTranslation() + glm::vec3{1.6f,-0.9f,0.9f}
    ,   {0,180,0}
    ,   default_scale
    ,   "assets/engine/missile-launcher.obj"
    );

        auto m = engine->createModel(
            nullptr
        ,   {0,0,0}
        ,   default_rot_and_scale
        ,   "assets/game/characters/melee/noanim.obj"
        );

    constexpr double const fpsLimit = 1.0 / 60.0;
    double lastUpdateTime = 0.0;  // number of seconds since the last loop
    double lastFrameTime = 0.0;   // number of seconds since the last frame
    float camSpeed = 10.0f;

    while(engine->isWindowActive())
    {
        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;
        float dt { static_cast<float>(deltaTime) }; 

        if ((now - lastFrameTime) >= fpsLimit)
        {
            if(engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
                engine->setWindowActive(false);

            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -camSpeed,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  camSpeed,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -camSpeed} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  camSpeed} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  camSpeed,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -camSpeed,  0.0f} * dt);  

            camnode->setCameraTarget({0,0,0});
            engine->updatePhysics(deltaTime);

            lastFrameTime = now;
        }

        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        lastUpdateTime = now;
    }

}

void test_particle_system_2(void) {
    auto engine = std::make_unique<hyen::HyperEngine>(true);
    engine->setWindowTitle("test_particle_system_2");
    engine->setWindowIcon("assets/engine/logo.jpg");

    [[maybe_unused]] 
    hyen::Node* camnode = engine->createCamera(
            nullptr
        ,   {0,0,-4}
        ,   default_rot_and_scale
    ); // tendrá la proyección por defecto    

    hyen::Node* lightnode = engine->createLight(
            nullptr
        ,   {0,0,0}
        ,   default_rot_and_scale
        ,   hyen::LightType::Point
        ,   hyen::LightIntensity   { .ambient{0.25f, 0.25f, 0.25f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
        ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.00009}, .quadratic{0.000032} }
        ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
        ,   hyen::LightDirection   { 0,0,0 }
    );

    [[maybe_unused]] 
    hyen::Node* plane = engine->createModel(
            nullptr
        ,   {0,0,0}
        ,   {0,0,0}
        ,   default_scale
        ,   "assets/engine/planes/semicube.obj"
    );

    [[maybe_unused]]
    hyen::Node* missile_launcher = engine->createModel(
            camnode
        ,   camnode->getTranslation() + glm::vec3{1.5,-.8,0}
        ,   {0,180,0}
        ,   default_scale
        ,   "assets/engine/missile-launcher.obj"
    );

    hyen::ParticleGenerator::CInfo cInfo
    {
        .maxParticles{10000}
    ,   .texturePath{"assets/engine/particles/particle.DDS"}
    ,   .origin{0,0,0}
    // ,   .gravity{0.0f, -9.81f, 0.0f}
    ,   .gravity{0.0f, 0.0f, 0.0f}
    ,   .mainDir{0.0f, 10.0f, 0.0f}
    ,   .particlesPerSecond{100.0f}
    ,   .spreadFactor{1.5f}
    ,   .lifeSpan{5.0f}
    ,   .minParticleSize{0.15f}
    ,   .maxParticleSize{0.5f}
    ,   .shapeRadius{3.f}
    // ,   .funcColor{&hyen::PGF::generateParticleColorsStandard}
    ,   .funcColor{&hyen::PGF::generateParticleColorsRandomly}
    // ,   .funcColor{&hyen::PGF::generateParticleColorsAndTransparencyRandomly}
    ,   .funcSize{&hyen::PGF::generateParticleSizeBetween}
    ,   .funcRandomdir{&hyen::PGF::generateRandomDirectionSoftInfluence}
    ,   .funcPos{&hyen::PGF::generatePositionCameraTarget}
    ,   .funcMaindir{&hyen::PGF::generateMainDirectionCameraTarget}
    };

    // hyen::ParticleGenerator::CInfo cInfo
    // {
    //     .maxParticles{10000}
    // ,   .texturePath{"assets/particles/particle.DDS"}
    // ,   .origin{0,0,0}
    // ,   .gravity{0.0f, 0.0f, 0.0f}
    // ,   .mainDir{0.0f, 0.0f, 0.0f}
    // ,   .particlesPerSecond{100.0f}
    // ,   .spreadFactor{1.5f}
    // ,   .lifeSpan{5.0f}
    // ,   .minParticleSize{0.15f}
    // ,   .maxParticleSize{0.5f}
    // ,   .shapeRadius{5.f}
    // ,   .funcColor{&hyen::PGF::generateParticleColorsRandomly}
    // ,   .funcSize{&hyen::PGF::generateParticleSizeBetween}
    // ,   .funcRandomdir{&hyen::PGF::generateRandomDirectionStandard}
    // ,   .funcPos{&hyen::PGF::generateRandomPositionBoxShape}
    // ,   .funcMaindir{&hyen::PGF::generateMainDirectionStandard}
    // };

    // for (size_t i = 0; i < 50; i++)
    // {
    //     generators.push_back(engine->createParticleGenerator(cInfo));
    // }
    

    auto generator = engine->createParticleGenerator(cInfo);
    generator->setActive(true, true, 3.0f);
    // auto generator2 = engine->createParticleGenerator(cInfo);

    const double fpsLimit = 1.0 / 60.0;
    double lastUpdateTime = 0;  // number of seconds since the last loop
    double lastFrameTime = 0;   // number of seconds since the last frame

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;
        float dt { static_cast<float>(deltaTime) }; 

        // This if-statement only executes once every 60th of a second
        if ((now - lastFrameTime) >= fpsLimit)
        {
            // Input controls, updatin positions
            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))            camnode->translate(glm::vec3{ -5.0f,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))           camnode->translate(glm::vec3{  5.0f,  0.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_UP))              camnode->translate(glm::vec3{  0.0f,  0.0f, -5.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))            camnode->translate(glm::vec3{  0.0f,  0.0f,  5.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))           camnode->translate(glm::vec3{  0.0f,  5.0f,  0.0f} * dt);
            if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))    camnode->translate(glm::vec3{  0.0f, -5.0f,  0.0f} * dt);
            
            auto const& pos { camnode->getTranslation() + glm::vec3{0,0,10} };

            if(generator) generator->setOrigin({0,0,0});
            



            if(engine->getKeyContinuousPress(GLFW_KEY_KP_1))
            { 
                // generator->setGravity({0,0,0});
                if(generator) generator->setFuncPos(&hyen::PGF::generatePositionStatic);
                if(generator) generator->setFuncMaindir(&hyen::PGF::generateMainDirectionStandard);
                if(generator) generator->setOrigin(pos);
            }

            if(engine->getKeyContinuousPress(GLFW_KEY_KP_2))
            {
                if(generator) generator->setActive(true);
            }

            if(engine->getKeyContinuousPress(GLFW_KEY_KP_3))
            {
                if(generator) generator->setActive(false);
            }

            if(engine->getKeySinglePress(GLFW_KEY_KP_4))
            {
                if(generator) delete generator;
                generator = nullptr;
            }


            // updating stuff
            camnode->setCameraTarget({0,0,0});
            engine->updatePhysics(deltaTime);

            if(generator) generator->setMatrices(camnode);
            // generator2->setMatrices(camnode);

            if(generator) generator->update(dt);
            // generator2->update(dt);

            lastFrameTime = now;
        }

        // Render
        engine->beginRender();
        engine->drawScene();

        if(generator) generator->render();
        // generator2->render();

        engine->endRender();

        lastUpdateTime = now;
    }

    delete generator;
}

void test_physics_world_importer(void) {
    auto engine = std::make_unique<hyen::HyperEngine>(true);
    engine->setWindowTitle("test_physics_world_importer");
    engine->setWindowIcon("assets/engine/logo.jpg");

    [[maybe_unused]] 
    hyen::Node* camnode = engine->createCamera(
            nullptr
        ,   {-5,2,0}
        ,   default_rot_and_scale
    ); // tendrá la proyección por defecto    

    hyen::Node* lightnode = engine->createLight(
            default_createnode_params
        ,   hyen::LightType::Point
        ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
        ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.09}, .quadratic{0.032} }
        ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
        ,   hyen::LightDirection   { 0,0,0 }
    );

    hyen::Node* lightnode3 = engine->createLight(
            nullptr
        ,   glm::vec3(2.0f,-3.5f,0.0f)
        ,   default_rot_and_scale   
        ,   hyen::LightType::Point
        ,   hyen::LightIntensity   { .ambient{0.02f, 0.02f, 0.2f}, .diffuse{0.8f, 0.8f, 0.8f}, .specular{1.0f, 1.0f, 1.0f} }
        ,   hyen::LightAttenuation { .constant{1.0f}, .linear{0.009f}, .quadratic{0.00032f} }
        ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
        ,   hyen::LightDirection   { 0,0,0 }
    );

    hyen::Node* lightnode2 = engine->createLight(
            default_createnode_params
        ,   hyen::LightType::Directional
        ,   hyen::LightIntensity   { .ambient{0.05f, 0.05f, 0.05f}, .diffuse{0.4f, 0.4f, 0.4f}, .specular{0.5f, 0.5f, 0.5f} }
        ,   hyen::LightAttenuation { .constant{0.0f}, .linear{0.0f}, .quadratic{0.0f} }
        ,   hyen::LightAperture    { .innerCutoff{0.0f}, .outerCutoff{0.0f} }
        ,   hyen::LightDirection   { -0.2f, -1.0f, -0.3f }
    );

    

    [[maybe_unused]] 
    hyen::Node* plane = engine->createModel(
            nullptr
        ,   {0,-0.2,0}
        ,   {0,0,0}
        ,   default_scale
        // ,   "assets/planes/semicube.obj"
        ,   "assets/engine/planes/character-test.obj"
        // ,   "assets/mapa1joined.obj"
        // ,   "assets/pruebacolisiones.obj"
        // ,   "assets/planes/mapa3.obj"
        // ,   "assets/planes/halllway.obj"
    );

    [[maybe_unused]] 
    hyen::Node* cubito_rosa = engine->createModel(
            nullptr
        ,   {0,1,0}
        ,   default_rot
        ,   {0.2f,0.2f,0.2f}
        ,   "assets/engine/cubito_rosa.obj"
    );

    engine->enableDebugDraw();
    plane->setNameID(1);
    cubito_rosa->setNameID(2);

    engine->createPhysicPropertiesTriangleMeshShape(
            plane
        ,   0
        ,   8
        ,   -1
    );

    // engine->createPhysicPropertiesFromArchive(
    //         plane
    //     // ,   "assets/physics/fisica.bullet"
    //     ,   "assets/physics/mapa1.bullet"
    //     // ,   "assets/physics/entrenamiento.bullet"
    //     // ,   "assets/physics/mapa1.dae"
    // );

    // Datos para Liam
    engine->createPhysicPropertiesKinematicCharacterController(
            cubito_rosa
        ,   0.5f
        ,   1.8f
        ,   2.0f
        ,   0.9f
        ,   -1
    );

    auto& controller = cubito_rosa->getPhysicProperties()->charCon;
    controller->setLinearVelocity({0.00001f, 0.0f, 0.00001f});

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        // Render
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        // Input controls
        if(engine->getKeyContinuousPress(GLFW_KEY_A))       
            camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))       
            camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))       
            camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))       
            camnode->rotate({-3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    
            camnode->translate({-.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   
            camnode->translate({.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_UP))      
            camnode->translate({0,0,-.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    
            camnode->translate({0,0,.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_SPACE))       
            camnode->translate({0,.3f,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT_CONTROL))       
            camnode->translate({0,-.3f,0});

        // Input debug
        if(engine->getKeySinglePress(GLFW_KEY_0))
        {
            engine->deletePhysicProperties(cubito_rosa);
            engine->deletePhysicProperties(plane);
        }

        if(engine->getKeyContinuousPress(GLFW_KEY_2))
        {
            engine->deleteAllWorldPhysics();
        }

        if(engine->getKeyContinuousPress(GLFW_KEY_KP_1))
        {
            if(controller->onGround())
            {
                controller->jump({0,5,0});
            }
        }

        // controller->setWalkDirection({0,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_KP_8))
            controller->setWalkDirection(btVector3(0,0,1).normalized() / 10);
        if(engine->getKeyContinuousPress(GLFW_KEY_KP_2))
            controller->setWalkDirection(btVector3(0,0,-1).normalized() / 10);
        if(engine->getKeyContinuousPress(GLFW_KEY_KP_4))
            controller->setWalkDirection(btVector3(1,0,0).normalized() / 10);
        if(engine->getKeyContinuousPress(GLFW_KEY_KP_6))
            controller->setWalkDirection(btVector3(-1,0,0).normalized() / 10);

        // updating stuff
        camnode->setCameraTarget({0,0,0});
        engine->updatePhysics();
    }
}

int main(void)
{
    // test_frustum();
    test_normal_mapping();
    // test_particle_system_2();
    // test_physics_world_importer();
}
