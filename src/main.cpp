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

	auto shaderID = ResourceManager::getResource_t<RShader>("src/shaders/1.model_loading")->getProgramID();

    Node* node = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    Node* node2 = engine->createModel(nullptr, {2.0f, 0.0f, 0.0f}, default_rot, default_scale, "assets/HA_funador_pesado.obj");
    node->getEntity()->setProgramID(shaderID);
    node2->getEntity()->setProgramID(shaderID);

    while(engine->isWindowActive() && !engine->isKeyPressed(GLFW_KEY_ESCAPE))
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
    LOG(glm::to_string(m));
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
    std::unique_ptr<hyper::HyperEngine> engine = std::make_unique<hyper::HyperEngine>(true);
	auto shaderID = hyper::ResourceManager::getResource_t<hyper::RShader>("src/shaders/model-loading-m-v-p")->getProgramID();

    auto camnode = engine->createCamera(nullptr, default_matrix_params); // tendrá la proyección por defecto
    camnode->getEntity()->setProgramID(shaderID);
    camnode->translate({0,0,4});

    hyper::Node* missile_launcher = engine->createModel(default_createnode_params, "assets/missile-launcher.obj");
    hyper::Node* funador_pesado = engine->createModel(nullptr, {2.0f, 0.0f, 0.0f}, default_rot, default_scale, "assets/HA_funador_pesado.obj");
    missile_launcher->getEntity()->setProgramID(shaderID);
    funador_pesado->getEntity()->setProgramID(shaderID);

    missile_launcher->translate({0.0f,0.0f,-3.0f});

    while(engine->isWindowActive() && !engine->isKeyPressed(GLFW_KEY_ESCAPE))
    {
        engine->beginRender();
        engine->drawScene();
        engine->endRender();

        if(engine->isKeyPressed(GLFW_KEY_A))
            camnode->rotate({0,3,0});
        if(engine->isKeyPressed(GLFW_KEY_D))
            camnode->rotate({0,-3,0});
        if(engine->isKeyPressed(GLFW_KEY_W))
            camnode->rotate({3,0,0});
        if(engine->isKeyPressed(GLFW_KEY_S))
            camnode->rotate({-3,0,0});

        if(engine->isKeyPressed(GLFW_KEY_UP))
            camnode->translate({0,0.1f,0});
        if(engine->isKeyPressed(GLFW_KEY_DOWN))
            camnode->translate({0,-0.1f,0});
        if(engine->isKeyPressed(GLFW_KEY_LEFT))
            camnode->translate({-0.1f,0,0});
        if(engine->isKeyPressed(GLFW_KEY_RIGHT))
            camnode->translate({0.1f,0,0});
    }

}

int main(void) {
	// test_models_and_imgui();
	// test_basic_lights();
    // test_view_matrix_inverse();

    // test_hyperengine_traverse();

    test_full_tree_traverse();
}
