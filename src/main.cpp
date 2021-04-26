/*
    This file exists only for testing
    it'll be removed once the engine
    is compiled into an .a file
*/
#include <main.hpp>

// void test_models_and_imgui(void) {
//     auto engine = new HyperEngine;
//     engine->initialize();
// 	auto window = engine->getWindow();

// 	// Imgui setup
// 	IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     ImGui::StyleColorsDark();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 130");
// 	bool show_another_window { true };
// 	glm::vec3 rspeed {0,0,0};
// 	// end (Imgui setup)

// 	auto rootnode = new Node();
// 	auto programID = ResourceManager::getResource_t<RShader>("src/shaders/1.model_loading")->getProgramID();

//     // Node* node = engine->create<EModel>(rootnode, default_matrix_params, "assets/missile-launcher.obj");
//     Node* node = engine->createModel(rootnode, default_matrix_params, "assets/missile-launcher.obj");
//     node->getEntity()->setProgramID(programID);
//     Node* node2 = engine->createModel(rootnode, {1.0f, 0.0f, 0.0f}, default_rot, default_scale, "assets/HA_funador_pesado.obj");
//     node2->getEntity()->setProgramID(programID);

// 	// Node* node = new Node();
// 	// EModel* modelEntity = new EModel("assets/missile-launcher.obj");

// 	// // "assets/pruebastexturas/cube_hardbytes.obj"
// 	// // "assets/pruebastexturas/cubo_substance.obj"
// 	// modelEntity->setProgramID(programID);
// 	// node->setEntity(modelEntity);
// 	// rootnode->addChild(node);

// 	// Node* node2 = new Node();
// 	// EModel* modelEntity2 = new EModel("assets/HA_funador_pesado.obj");
// 	// modelEntity2->setProgramID(programID);
// 	// node2->setEntity(modelEntity2);
// 	// node2->setTranslation({-2,0,0});
// 	// rootnode->addChild(node2);

//     // Node* node2 = engine->createModel(rootnode, {0,0,0}, {0,0,0}, {1,1,1}, "assets/HA_funador_pesado.obj");

// 	do{
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		rootnode->traverse(MVP);

// 		ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

// 		if(show_another_window) {
// 			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
// 			ImGui::Text("Hello from another window!");
// 			// Drags
// 			ImGui::Text("Velocidad de rotación en X: %f", rspeed.x);
// 			ImGui::DragFloat("rspeed.x (-0.1 -> +0.1)", &rspeed.x, 0.003f, -2.0f, 2.0f, "%.3f", 0);
// 			ImGui::Text("Velocidad de rotación en Y: %f", rspeed.y);
// 			ImGui::DragFloat("rspeed.y (-0.1 -> +0.1)", &rspeed.y, 0.003f, -2.0f, 2.0f, "%.3f", 0);
// 			ImGui::Text("Velocidad de rotación en Z: %f", rspeed.z);
// 			ImGui::DragFloat("rspeed.z (-0.1 -> +0.1)", &rspeed.z, 0.003f, -2.0f, 2.0f, "%.3f", 0);
// 			if (ImGui::Button("Ciérrame"))
// 				show_another_window = false;
// 			ImGui::End();
// 		}

// 		ImGui::Render();
// 		int display_w, display_h;
//         glfwGetFramebufferSize(window, &display_w, &display_h);
//         glViewport(0, 0, display_w, display_h);
// 		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// 		glfwSwapBuffers(window);
// 		glfwPollEvents();

// 		rootnode->rotate(rspeed);
// 	}
// 	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
// 		   glfwWindowShouldClose(window) == 0 );

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
// 	glfwDestroyWindow(window);
// 	glfwTerminate();

// 	if(rootnode) delete rootnode;
//     if(engine) delete engine;
// }

void test_basic_lights() {
    using namespace hyper;
    auto engine = new HyperEngine;
    engine->initialize();
	auto window = engine->getWindow();
	// build and compile our shader zprogram
    // ------------------------------------

	auto& lightingShader = *ResourceManager::getResource_t<RShader>("src/shaders/2.2.basic_lighting");
	auto& lightCubeShader = *ResourceManager::getResource_t<RShader>("src/shaders/2.2.light_cube");
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 cameraPosition(-2.f,-2.f,3.f);
	auto cameraViewMatrix = glm::lookAt(
		cameraPosition
		// ,	cameraPosition + glm::vec3(0,0,-1)
		, {0,0,0}
		,	glm::vec3(0,1,0)
	);

	float cameraZoom { 45.0f };


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS)
    {
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.bind();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", cameraPosition);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(cameraZoom), (float)1366 / (float)768, 0.1f, 100.0f);
        glm::mat4 view = cameraViewMatrix;
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        lightCubeShader.bind();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void test_hyperengine_traverse(void) {
    using namespace hyper;
    auto engine = new HyperEngine(true);

    auto shader = ResourceManager::getResource_t<RShader>("src/shaders/1.model_loading");
	[[maybe_unused]] auto shaderID = shader->getProgramID();

    Node* node = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    Node* node2 = engine->createModel(nullptr, {2.0f, 0.0f, 0.0f}, default_rot, default_scale, "assets/HA_funador_pesado.obj");
    node->getEntity()->setShader(shader);
    node2->getEntity()->setShader(shader);

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        node->rotate({0.0f,1.0f,0.0f});
    }

    delete engine;
}

void printMat4(glm::mat4 const& m)
{
    INFOLOG("Matriz entera de transformación: " << glm::to_string(m));
}

void test_view_matrix_inverse(void) {
    // auto engine = new HyperEngine(true);
	// auto window = engine->getWindow();

    // auto root = new Node;

    // Node* cameranode = new Node;
    // cameranode->setTranslation({0, 1.f, 3.f});
    // cameranode->setRotation({-20,0,0});
    // cameranode->setScale({1,1,1});
    // auto viewmatrix = cameranode->getUpdatedMatrixTransform();
    // auto newMVP =  Projection * glm::inverse(viewmatrix) * Model;

    // auto programID = ResourceManager::getResource_t<RShader>("src/shaders/1.model_loading")->getProgramID();

    // Node* node = engine->createModel(root, default_matrix_params, "assets/missile-launcher.obj");
    // node->getEntity()->setProgramID(programID);

    // INFOLOG("Matriz vista original:: ");
    // printMat4(View);
    // INFOLOG("Matriz vista nueva:: ");
    // printMat4(glm::inverse(viewmatrix));

	// do{
	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     node->traverse(newMVP);

	// 	glfwSwapBuffers(window);
	// 	glfwPollEvents();
	// }
	// while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	// 	   glfwWindowShouldClose(window) == 0 );

	// glfwDestroyWindow(window);
	// glfwTerminate();

    // if(engine) delete engine;
}

void test_full_tree_traverse() {
    // No son necesarios los parámetros de shader, lo que hacen es sobreescribir el shader con el que se dibujan
    // pero la engine les pone un shader por defecto

	// auto shaderID = hyper::ResourceManager::getResource_t<hyper::RShader>("src/shaders/model-loading-m-v-p")->getProgramID();
    // camnode->getEntity()->setProgramID(shaderID);
    // missile_launcher->getEntity()->setProgramID(shaderID);
    // funador_pesado->getEntity()->setProgramID(shaderID);

    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);

    auto camnode = engine->createCamera(default_createnode_params); // tendrá la proyección por defecto

    hyper::Node* missile_launcher = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    [[maybe_unused]] hyper::Node* funador_pesado = engine->createModel(nullptr, {2.0f, 0.0f, 0.0f}, default_rot, default_scale, "assets/HA_funador_pesado.obj");

    camnode->translate({0,0,4});
    missile_launcher->translate({0.0f,0.0f,-3.0f});

    glm::vec3* misiletrans  = { new glm::vec3( missile_launcher->getTranslation() )};
    glm::vec3* misilerot    = { new glm::vec3( missile_launcher->getRotation() )};

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        engine->beginRender();
        engine->drawScene();

        // Renderizar lo que sea de la gui entre beginRender y endRender
        // HyperEngine::drawExampleWindowGUI() solo hace una ventana de ejemplo, lo suyo es hacer begin, text, end, las cosas de imgui
        // engine->drawExampleWindowGUI();

        hyper::gui::Begin("Ventana temporal2 - HyperEngine::drawExampleWindowGUI()", 0,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	    hyper::gui::Button("Hola mundo2!");
        ImGui::SliderFloat3("Lanzamisiles - Translación", (float*)misiletrans, -10.0f, 10.0f);
        ImGui::SliderFloat3("Lanzamisiles - Rotación   ", (float*)misilerot, -180.0f, 180.0f);
	    hyper::gui::End();

        engine->endRender();

        if(engine->getKeyContinuousPress(GLFW_KEY_A))
            camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))
            camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))
            camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))
            camnode->rotate({-3,0,0});

        if(engine->getKeyContinuousPress(GLFW_KEY_UP))
            camnode->translate({0,0.1f,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))
            camnode->translate({0,-0.1f,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))
            camnode->translate({-0.1f,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))
            camnode->translate({0.1f,0,0});

        missile_launcher->setTranslation( *(glm::vec3*)misiletrans );
        missile_launcher->setRotation( *(glm::vec3*)misilerot );
    }

}

void test_input_callbacks() {
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);

    hyper::Node* camnode = engine->createCamera(nullptr,{1.0f,0.0f,4.0f},{0,15,0} , default_scale); // tendrá la proyección por defecto
    // hyper::Node* lightnode = engine->createLight(default_createnode_params);
    hyper::Node* missile_launcher = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    // hyper::Node* funador_pesado = engine->createModel(nullptr, {1.0f, 0.0f, 0.0f}, default_rot_and_scale, "assets/HA_funador_pesado.obj");
    hyper::Node* funador_pesado2 = engine->createModel(nullptr, {-1.0f, 0.0f, 1.0f}, default_rot_and_scale, "assets/newmachinegun/HA_funador_pesado.obj");
    missile_launcher = funador_pesado2;

    hyper::Node* cubito_rosa = engine->createModel(default_createnode_params, "assets/cubito_rosa.obj");
    hyper::Node* icosphere = engine->createModel(default_createnode_params, "assets/icosphere.obj");
    cubito_rosa->setTranslation({-2.0f,0.0f,0.0f});
    cubito_rosa->setScale({0.3f,0.3f,0.3f});
    icosphere->setTranslation({2,0,0});

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        missile_launcher->rotate({0,5.5f,0});

        if(engine->getKeyContinuousPress(GLFW_KEY_A))
            camnode->rotate({0,3,0});

        if(engine->getKeyKeyboardPress(GLFW_KEY_D))
            camnode->rotate({0,-3,0});

        if(engine->getKeySinglePress(GLFW_KEY_W))
            camnode->rotate({3,0,0});

        if(engine->getKeyRelease(GLFW_KEY_S))
            camnode->rotate({-3,0,0});

        if(engine->getKeySinglePress(GLFW_KEY_1))
            engine->clearTree();

        if(engine->getKeySinglePress(GLFW_KEY_2) && engine->isTreeEmpty())
        {
            camnode = engine->createCamera(nullptr,{1.0f,0.0f,4.0f},{0,15,0} , default_scale); // tendrá la proyección por defecto
            missile_launcher = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
            cubito_rosa = engine->createModel(default_createnode_params, "assets/cubito_rosa.obj");
            icosphere = engine->createModel(default_createnode_params, "assets/icosphere.obj");
        }

        if(engine->getKeySinglePress(GLFW_KEY_3))
        {
            INFOLOG("ID del nodo camnode "<<VAR(camnode->getNameID()));
            INFOLOG("ID del nodo camnode "<<VAR(camnode->getNameID()));
            INFOLOG("ID del nodo missile_launcher "<<VAR(missile_launcher->getNameID()));
            INFOLOG("ID del nodo cubito_rosa "<<VAR(cubito_rosa->getNameID()));
            INFOLOG("ID del nodo icosphere "<<VAR(icosphere->getNameID()));
        }

        if(engine->getKeySinglePress(GLFW_KEY_4))
            engine->setWindowClearColor(0,0,0,0); // Negro

        if(engine->getKeySinglePress(GLFW_KEY_5))
            engine->setWindowTitle("Looooooooooooooooooooooooool");
        if(engine->getKeySinglePress(GLFW_KEY_6))
            engine->setWindowIcon("assets/logo.jpg");

        // Testing cursor, still in progress (try using cursor callback and disabling cursor input)
        if(engine->getKeySinglePress(GLFW_KEY_7))
            engine->setCursorVisibility(true);
        if(engine->getKeySinglePress(GLFW_KEY_8))
            engine->setCursorVisibility(false);

        if(engine->getKeySinglePress(GLFW_KEY_9))
            engine->setCursorPosition();


        // if(engine->isKeyPressed(GLFW_KEY_UP))
        //     camnode->translate({0,0.1f,0});
        // if(engine->isKeyPressed(GLFW_KEY_DOWN))
        //     camnode->translate({0,-0.1f,0});
        // if(engine->isKeyPressed(GLFW_KEY_LEFT))
        //     camnode->translate({-0.1f,0,0});
        // if(engine->isKeyPressed(GLFW_KEY_RIGHT))
        //     camnode->translate({0.1f,0,0});

    }

}

void test_matrices_data_lights(void) {
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
    engine->setWindowTitle("test_matrices_data_lights");
    engine->setWindowIcon("assets/logo.jpg");

    [[maybe_unused]] hyper::Node* lightnode = engine->createLight(default_createnode_params);
    hyper::Node* camnode = engine->createCamera(nullptr,{1.0f,0.0f,4.0f},{0,15,0} , default_scale); // tendrá la proyección por defecto
    hyper::Node* missile_launcher = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    // hyper::Node* funador_pesado2 = engine->createModel(nullptr, {-1.0f, 0.0f, 1.0f}, default_rot_and_scale, "assets/newmachinegun/HA_funador_pesado.obj");
    hyper::Node* cubito_rosa = engine->createModel(default_createnode_params, "assets/cubito_rosa.obj");
    hyper::Node* icosphere = engine->createModel(default_createnode_params, "assets/icosphere.obj");

    cubito_rosa->setTranslation({-2.0f,0.0f,0.0f});
    cubito_rosa->setScale({0.3f,0.3f,0.3f});
    icosphere->setTranslation({2,0,0});

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        // auto trans = camnode->getMatrixTransform();
        // glm::vec4 result = trans * glm::vec4(0,0,0,1); // saca la última fila de datos, que son los de translación (posición)
        // INFOLOG(glm::to_string(result))
        // INFOLOG(glm::to_string(camnode->getTranslation()))

        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        missile_launcher->rotate({0,5.5f,0});

        if(engine->getKeyContinuousPress(GLFW_KEY_A))       camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))       camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))       camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))       camnode->rotate({-3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    camnode->translate({-.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   camnode->translate({.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_UP))      camnode->translate({0,0,-.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    camnode->translate({0,0,.3f});

        if(engine->getKeySinglePress(GLFW_KEY_1))       engine->enableCulling();
        if(engine->getKeySinglePress(GLFW_KEY_2))       engine->disableCulling();

        if(engine->getKeySinglePress(GLFW_KEY_3))       glCullFace(GL_FRONT);
        if(engine->getKeySinglePress(GLFW_KEY_4))       glCullFace(GL_BACK);
        if(engine->getKeySinglePress(GLFW_KEY_5))       glCullFace(GL_FRONT_AND_BACK);
        if(engine->getKeySinglePress(GLFW_KEY_6))       glFrontFace(GL_CW);
        if(engine->getKeySinglePress(GLFW_KEY_7))       glFrontFace(GL_CCW);




    }
}

void test_collisions_bullet(void) {
    // std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
    // engine->setWindowTitle("test_collisions_bullet");
    // engine->setWindowIcon("assets/logo.jpg");

    // hyper::Node* camnode = engine->createCamera(nullptr, {0.0f,0.0f,5.0f}, default_rot_and_scale); // tendrá la proyección por defecto
    // hyper::Node* missile_launcher = engine->createModel(nullptr, {0,1,0}, default_rot_and_scale, "assets/missile-launcher.obj");
    // hyper::Node* floor = engine->createModel(nullptr, {0, -1, 0}, default_rot_and_scale, "assets/planes/plano1.obj");

    // CollisionManager* collisionManager { new CollisionManager };
	// btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
	// collisionManager->m_collisionShapes.push_back(groundShape);


    // btVector3 floorPosition = { floor->getTranslation().x, floor->getTranslation().y, floor->getTranslation().z };
	// btTransform groundTransform;
	// groundTransform.setIdentity();
	// groundTransform.setOrigin(floorPosition);      // btVector3(0,-50,0)

    // {
	// 	btScalar mass(0.);

	// 	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 	bool isDynamic = (mass != 0.f);

	// 	btVector3 localInertia(0,0,0);
	// 	if (isDynamic)
	// 		groundShape->calculateLocalInertia(mass,localInertia);

	// 	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	// 	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	// 	btRigidBody* body = new btRigidBody(rbInfo);

	// 	//add the body to the dynamics world
	// 	collisionManager->m_dynamicsWorld->addRigidBody(body);
	// }

    // while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    // {
    //     engine->beginRender();
    //     engine->drawScene();

    //     collisionManager->m_dynamicsWorld->debugDrawWorld();

    //     engine->endRender();

    //     missile_launcher->rotate({0,2,0});

    //     if(engine->getKeyContinuousPress(GLFW_KEY_A))       camnode->rotate({0,3,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_D))       camnode->rotate({0,-3,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_W))       camnode->rotate({3,0,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_S))       camnode->rotate({-3,0,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    camnode->translate({-.3,0,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   camnode->translate({.3,0,0});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_UP))      camnode->translate({0,0,-.3f});
    //     if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    camnode->translate({0,0,.3f});
    // }
}

void test_skybox_and_mouse_input() {
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
    engine->setWindowTitle("test_skybox_and_mouse_input");
    engine->setWindowIcon("assets/logo.jpg");

    [[maybe_unused]] hyper::Node* lightnode = engine->createLight(default_createnode_params);
    hyper::Node* camnode = engine->createCamera(default_createnode_params); // tendrá la proyección por defecto

    hyper::Node* missile_launcher = engine->createModel(nullptr, {0,0,-4}, default_rot_and_scale, "assets/missile-launcher.obj");
    [[maybe_unused]] hyper::Node* cubito_rosa = engine->createModel(nullptr, {-2,0,-4}, default_rot_and_scale, "assets/cubito_rosa.obj");
    [[maybe_unused]] hyper::Node* icosphere = engine->createModel(nullptr, {2,0,-4}, default_rot_and_scale, "assets/icosphere.obj");

    // a medias de la skybox, hacer lo de los shaders y acordarse de cambiar la depth function (Z buffer) para que siempre se vea al fondo
    [[maybe_unused]] hyper::Node* skyboxnode = engine->createSkybox(
        default_createnode_params
    ,   SkyboxNamelist 
        { 
            "assets/skybox/space/lightblue/top.png"
        ,   "assets/skybox/space/lightblue/bot.png"
        ,   "assets/skybox/space/lightblue/left.png"
        ,   "assets/skybox/space/lightblue/right.png"
        ,   "assets/skybox/space/lightblue/front.png"
        ,   "assets/skybox/space/lightblue/back.png" 
        }
    );

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        // Render
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        // Fancy rotation
        missile_launcher->rotate({0,5.5f,0});

        // Input
        if(engine->getKeyContinuousPress(GLFW_KEY_A))       camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))       camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))       camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))       camnode->rotate({-3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    camnode->translate({-.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   camnode->translate({.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_UP))      camnode->translate({0,0,-.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    camnode->translate({0,0,.3f});

        if(engine->getKeySinglePress(GLFW_KEY_1))
        {
            auto pos = engine->getMousePositionAbsolute();
            INFOLOG("Posición del ratón: " << VAR(pos.x) << VAR(pos.y) );
        }

        if(engine->getKeySinglePress(GLFW_KEY_2))
        {
            auto pos = engine->getWindowSize();
            INFOLOG("Tamaño de ventana: " << VAR(pos.x) << VAR(pos.y) );
        }

        if(engine->getKeySinglePress(GLFW_KEY_3))
        {
            //  Viewport y window size son totalmente distintas cosas
            glViewport(0,0, 1600, 900);
        }

        if(engine->getKeySinglePress(GLFW_KEY_4))
        {
            engine->setWindowSize(1600, 900);
        }

        if(engine->getKeySinglePress(GLFW_KEY_5))
        {
            engine->setMousePositionToCenter();
        }

        if(engine->getKeySinglePress(GLFW_KEY_6))
        {
            static bool trigger = true;
            trigger = !trigger;
            engine->setCursorVisibility(trigger);
        }

        if(engine->getKeySinglePress(GLFW_KEY_7))
        {
            // Diferencia entre centro y posición del mouse correcta
            auto center = engine->getWindowSizeCenter();
            auto pos = engine->getMousePositionAbsolute();
            auto diff = center - glm::ivec2(pos.x, pos.y);

            INFOLOG("Diff: " VAR(diff.x) << VAR(diff.y) )
        }
    }
}

void test_physics(void) {
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
    engine->setWindowTitle("test_physics");
    engine->setWindowIcon("assets/logo.jpg");

    [[maybe_unused]] hyper::Node* lightnode          = engine->createLight(default_createnode_params);
    hyper::Node* camnode            = engine->createCamera(default_createnode_params); // tendrá la proyección por defecto

    hyper::Node* missile_launcher   = engine->createModel(nullptr, {0,0,-4}, default_rot_and_scale, "assets/missile-launcher.obj");
    hyper::Node* cubito_rosa        = engine->createModel(nullptr, {-4,0,-4}, default_rot_and_scale, "assets/cubito_rosa.obj");
    hyper::Node* icosphere          = engine->createModel(nullptr, {4,0,-4}, default_rot_and_scale, "assets/icosphere.obj");
    hyper::Node* plane              = engine->createModel(nullptr, {0,-2.0f,0}, {0,0,0}, default_scale, "assets/planes/plano3.obj");
    // hyper::Node* plane              = nullptr;

    {
    // a medias de la skybox, hacer lo de los shaders y acordarse de cambiar la depth function (Z buffer) para que siempre se vea al fondo
    // hyper::Node* skyboxnode = engine->createSkybox(
    //     default_createnode_params
    // ,   SkyboxNamelist 
    //     { 
    //         "assets/skybox/space/lightblue/top.png"
    //     ,   "assets/skybox/space/lightblue/bot.png"
    //     ,   "assets/skybox/space/lightblue/left.png"
    //     ,   "assets/skybox/space/lightblue/right.png"
    //     ,   "assets/skybox/space/lightblue/front.png"
    //     ,   "assets/skybox/space/lightblue/back.png" 
    //     }
    // );

    // hyper::Node* skyboxnode = engine->createSkybox(
    //     default_createnode_params
    // ,   SkyboxNamelist 
    //     { 
    //         "assets/skybox/space/red/bkg1_top3.png"
    //     ,   "assets/skybox/space/red/bkg1_bottom4.png"
    //     ,   "assets/skybox/space/red/bkg1_left2.png"
    //     ,   "assets/skybox/space/red/bkg1_right1.png"
    //     ,   "assets/skybox/space/red/bkg1_front5.png"
    //     ,   "assets/skybox/space/red/bkg1_back6.png" 
    //     }
    // );

    // hyper::Node* skyboxnode = engine->createSkybox(
    //     default_createnode_params
    // ,   SkyboxNamelist 
    //     { 
    //         "assets/skybox/space/blue/bkg1_top.png"
    //     ,   "assets/skybox/space/blue/bkg1_bot.png"
    //     ,   "assets/skybox/space/blue/bkg1_left.png"
    //     ,   "assets/skybox/space/blue/bkg1_right.png"
    //     ,   "assets/skybox/space/blue/bkg1_front.png"
    //     ,   "assets/skybox/space/blue/bkg1_back.png" 
    //     }
    // );
    };

    // auto dbgdrw = engine->getDebugDrawer();
    // dbgdrw->setDebugMode( btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
    // INFOLOG("debug mode:" VAR(dbgdrw->getDebugMode()))

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        // Render
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        // Fancy rotation
        missile_launcher->rotate({0,3,0});
        cubito_rosa->rotate({3,0,0});
        icosphere->rotate({0,0,3});

        // Input
        if(engine->getKeyContinuousPress(GLFW_KEY_A))       camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))       camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))       camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))       camnode->rotate({-3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    camnode->translate({-.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   camnode->translate({.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_UP))      camnode->translate({0,0,-.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    camnode->translate({0,0,.3f});

        if(engine->getKeySinglePress(GLFW_KEY_1))
        {
            INFOLOG("Tratando de añadir colisiones a nodo missile_launcher");
            engine->createRigidbody(missile_launcher);
        }

        if(engine->getKeySinglePress(GLFW_KEY_2))
        {
            missile_launcher->translate({0,0,-2});
            auto rigidbody = missile_launcher->getPhysicProperties()->m_data.body;
            // Debug, TODO:: borrar
            INFOLOG("llego" );

            btTransform nodetransform;
            rigidbody->getMotionState()->getWorldTransform(nodetransform);
            auto pos = nodetransform.getOrigin();
            INFOLOG("Posición del nodo: " << VAR(pos.x()) << VAR(pos.y()) << VAR(pos.z()) );
        }

        if(engine->getKeySinglePress(GLFW_KEY_3))
            engine->enableDebugDraw();
        if(engine->getKeySinglePress(GLFW_KEY_4))
            engine->disableDebugDraw();

        if(engine->getKeySinglePress(GLFW_KEY_5))
        {
            // INFOLOG("Tratando de añadir colisiones a nodo missile_launcher");
            engine->createRigidBodyConvexHull(missile_launcher);
        }

        if(engine->getKeySinglePress(GLFW_KEY_6))
        {
            engine->createTriangleMeshShape(plane);
        }

        if(engine->getKeySinglePress(GLFW_KEY_7))
            engine->getDebugDrawer()->toggleDebugFlag(2);

        if(engine->getKeySinglePress(GLFW_KEY_8))
            engine->createRigidBodyDynamic();

        if(engine->getKeyContinuousPress(GLFW_KEY_9))
        {
            hyper::RayResult result;
            bool hashit = engine->throwRaycast({0,1,-10}, {0, 1, 25}, result);

            if(hashit)
            {
                auto point = result.hitPoint;
                INFOLOG("Sí le he dado con el raycast: " VAR(point.getX()) << VAR(point.getY()) << VAR(point.getZ()) )
            }
            else
            {
                INFOLOG("No le he dado con el raycast")
            }

        }

        if(engine->getKeySinglePress(GLFW_KEY_F))
        {
            [[maybe_unused]] hyper::Node* skyboxnode = engine->createSkybox(
                default_createnode_params
            ,   SkyboxNamelist 
                { 
                    "assets/skybox/space/lightblue/top.png"
                ,   "assets/skybox/space/lightblue/bot.png"
                ,   "assets/skybox/space/lightblue/left.png"
                ,   "assets/skybox/space/lightblue/right.png"
                ,   "assets/skybox/space/lightblue/front.png"
                ,   "assets/skybox/space/lightblue/back.png" 
                }
            );
        }

        // Update de las físicas
        engine->updatePhysics();
    }
}

void test_physics_2(void) {
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
    engine->setWindowTitle("test_physics_2");
    engine->setWindowIcon("assets/logo.jpg");

    [[maybe_unused]] 
    hyper::Node* lightnode          = engine->createLight(default_createnode_params);
    
    hyper::Node* camnode            = engine->createCamera(default_createnode_params); // tendrá la proyección por defecto
    
    hyper::Node* missile_launcher   = engine->createModel(nullptr, {0,0,-4}, default_rot_and_scale, "assets/missile-launcher.obj");
    hyper::Node* cubito_rosa        = engine->createModel(nullptr, {-4,4,-4},{10,10,10}, default_scale, "assets/cubito_rosa.obj");
    hyper::Node* icosphere          = engine->createModel(nullptr, {4,0,-4}, default_rot_and_scale, "assets/icosphere.obj");
    
    [[maybe_unused]] 
    hyper::Node* plane              = engine->createModel(nullptr, {0,-2.0f,0}, {0,0,0}, default_scale, "assets/planes/plano3-2.obj");

    cubito_rosa->setNameID(13);
    engine->enableDebugDraw();

    while(engine->isWindowActive() && !engine->getKeyContinuousPress(GLFW_KEY_ESCAPE))
    {
        // Render
        engine->beginRender();
        engine->drawScene();

        if(engine->getKeyContinuousPress(GLFW_KEY_3)) 
        {
            // Nota, el raycast no se puede ver si no se hace entre begin y end del render
            // Arreglar para tener una stack de rayos o algo así
            auto pos = cubito_rosa->getTranslation();
            hyper::RayResult result;

            auto hasHit = engine->throwRaycast(
                    {pos.x, -20, pos.z}
                ,   {pos.x, pos.y+100, pos.z}
                ,   result
            );

            if(hasHit)
            {
                INFOLOG("He dado!" << VAR(result.hitPoint.getX()) << VAR(result.hitPoint.getY()) << VAR(result.hitPoint.getZ()))
                INFOLOG("El nodo tiene índice: " << VAR(result.node->getNameID()) );
            }
            else
            {
                INFOLOG("No he dado?")
            }
        }

        engine->endRender();

        // Fancy rotation
        // missile_launcher->rotate({0,3,0});
        // cubito_rosa->rotate({3,0,0});
        icosphere->rotate({0,0,3});

        // Debug
        // auto& trans = missile_launcher->getMatrixTransform();
        // printMat4(trans);

        // Input
        if(engine->getKeyContinuousPress(GLFW_KEY_A))       camnode->rotate({0,3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_D))       camnode->rotate({0,-3,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_W))       camnode->rotate({3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_S))       camnode->rotate({-3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_LEFT))    camnode->translate({-.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_RIGHT))   camnode->translate({.3,0,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_UP))      camnode->translate({0,0,-.3f});
        if(engine->getKeyContinuousPress(GLFW_KEY_DOWN))    camnode->translate({0,0,.3f});

        if(engine->getKeyContinuousPress(GLFW_KEY_E))    camnode->translate({0,.3f,0});
        if(engine->getKeyContinuousPress(GLFW_KEY_R))    camnode->translate({0,-.3f,0});

        if(engine->getKeyContinuousPress(GLFW_KEY_T))    missile_launcher->rotate({0,3,0});


        if(engine->getKeySinglePress(GLFW_KEY_1))
        {
            auto pos = hyper::util::glmVec3TobtVec3(icosphere->getTranslation());

            engine->createPhysicPropertiesCollisionObject(
                    icosphere
                ,   new btSphereShape(2)
            );

            // engine->createPhysicProperties(
            //         icosphere
            //     ,   new btSphereShape(1)
            //     ,   0
            //     ,   btVector3(pos.x, pos.y, pos.z)
            // );
        }

        if(engine->getKeySinglePress(GLFW_KEY_2))    
        {
            // Test de objetos dinámicos

            auto pos = hyper::util::glmVec3TobtVec3(cubito_rosa->getTranslation());
            // engine->createPhysicProperties(
            //         cubito_rosa
            //     ,   new btBoxShape({1,1,1})
            //     ,   0
            //     ,   btVector3(pos.x, pos.y, pos.z)
            // );
            engine->createPhysicPropertiesDynamicBody(
                    cubito_rosa
                ,   new btBoxShape({1,1,1})
                ,   1
                ,   pos
            );
        }

        if(engine->getKeySinglePress(GLFW_KEY_4))
        {
            auto pos = plane->getTranslation();
            engine->createPhysicPropertiesTriangleMeshShape(
                    plane
                ,   0
                ,   hyper::util::glmVec3TobtVec3(pos)
            );

        }

        if(engine->getKeySinglePress(GLFW_KEY_5))
            engine->getDebugDrawer()->toggleDebugFlag(2);

        if(engine->getKeySinglePress(GLFW_KEY_6))
            engine->createRigidBodyDynamic();


        if(engine->getKeyContinuousPress(GLFW_KEY_7))
        {
            static int dir = 1;
            static int const threshold = 7;
            missile_launcher->translate({.1*dir,0,0});

            auto translation = missile_launcher->getTranslation();
            if(translation.z > threshold)
                dir=-1;
            else if(translation.z < -threshold)
                dir=1;
        }    
        if(engine->getKeyContinuousPress(GLFW_KEY_8))
        {
            auto pos = hyper::util::glmVec3TobtVec3(missile_launcher->getTranslation());
            engine->createPhysicPropertiesKinematicBody(
                    missile_launcher
                ,   new btBoxShape({.5,.8,1.5})
                // ,   new btCylinderShape({1,5,1})
                ,   pos
            );
        }

        if(engine->getKeyContinuousPress(GLFW_KEY_9))
        {
            auto prop = cubito_rosa->getPhysicProperties();
            prop->m_data.body->applyCentralForce({0,20,0});
        }

        if(engine->getKeyContinuousPress(GLFW_KEY_H)) 
        {
            missile_launcher->rotate({1,0,0});
        }
        if(engine->getKeyContinuousPress(GLFW_KEY_J)) 
        {
            missile_launcher->rotate({0,1,0});
        }
        if(engine->getKeyContinuousPress(GLFW_KEY_K)) 
        {
            missile_launcher->rotate({0,0,1});
        }

        // if(engine->getKeyContinuousPress(GLFW_KEY_L))
        // {
        //     auto target = camnode->getCameraTarget();
        //     target.y += 0;
        //     camnode->setCameraTarget(target);
        // }

        camnode->setCameraTarget({0,0,0});



        // Update de las físicas
        engine->updatePhysics();
    }
}

int main(void) {
	// test_models_and_imgui();
	// test_basic_lights();
    // test_view_matrix_inverse();

    // test_hyperengine_traverse();

    // test_full_tree_traverse();

    // test_input_callbacks();

    // test_matrices_data_lights();

    // test_collisions_bullet();

    // test_skybox_and_mouse_input();

    // test_physics();

    test_physics_2();
}
