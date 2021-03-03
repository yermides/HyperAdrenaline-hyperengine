/*
    This file exists only for testing
    it'll be removed once the engine
    is compiled into an .a file
*/
#include <main.hpp>

// Prueba de funcionamiento de los nodos
void tree_test(void) {
    ELight* light = new ELight();
    light->setAperture(0.0f);
    
    LOG("Hello world");
    LOG("The light has " << light->getAperture() << " aperture");

    Node* node = new Node();
    node->setEntity(light);

    // Tecnicamente es una matriz de unos
    glm::mat4 ones { 1.0f };    

    node->getEntity()->draw(ones);

    RShader* shader = new RShader();

    delete light;
    delete node;
    delete shader;
}

// Carga de cubo por vértices hardcoded, deben alterarse las funciones de RMesh::loadMesh y RMesh::draw
void cube_test(void) {
    GLFWwindow* window = hrn::initializeWindow();

	// Load a shader
	RShader* rshader = new RShader();

	// Create and compile our GLSL program from the shaders
	GLuint programID = rshader->loadShaders (
            "src/shaders/TransformVertexShader.vertexshader"
        ,   "src/shaders/ColorFragmentShader.fragmentshader" 
    );

	// Create a node
	Node* node = new Node();

	// Assign a model entity
	EModel* modelEntity = new EModel();
	node->setEntity(modelEntity);

	// Load a shader and a mesh in the entity
	modelEntity->setProgramID(programID);
	modelEntity->loadFromFile("cubo precioso");

	// MVP matrix obtenida en el hpp, son variables static para no manchar el main.cpp

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		node->traverse(MVP);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		if(glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS)
			node->rotate({1,0,0});
		if(glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS)
			node->scale({1,1,1});
		if(glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS)
			node->scale({-1,-1,-1});
		if(glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS)
			node->translate({0.2,0.2,0.2});
		if(glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS)
			node->setTranslation({0,0,0});
		if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
			node->setRotation({0,0,0});
		if(glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS)
			node->setScale({1,1,1});

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete rshader;
	delete node;
	delete modelEntity;
}

// Carga una isosphere (desde assets/) y la muestra con un color de shader básico
void loading_models_assimp_test(void) {
    GLFWwindow* window = hrn::initializeWindow();

	// Load a shader
	RShader* rshader = new RShader();
	// Create and compile our GLSL program from the shaders

	GLuint programID = rshader->loadShaders (
            "src/shaders/vertex.glsl"
        ,   "src/shaders/fragment.glsl" 
    );

	// Create a node
	Node* node = new Node();

	// Assign a model entity
	EModel* modelEntity = new EModel();
	modelEntity->setProgramID(programID);
	// modelEntity->loadFromFile("assets/cube.obj");
	// modelEntity->loadFromFile("assets/missile-launcher.obj");
	// modelEntity->loadFromFile("assets/icosphere.obj");
	modelEntity->loadFromFile("assets/test_gun.obj");
	
	node->setEntity(modelEntity);

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		node->traverse(MVP);

		if(glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS)
			node->rotate({1,0,0});
		if(glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS)
			node->scale({1,1,1});
		if(glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS)
			node->scale({-1,-1,-1});
		if(glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS)
			node->translate({0.2,0.2,0.2});
		if(glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS)
			node->setTranslation({0,0,0});
		if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
			node->setRotation({0,0,0});
		if(glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS)
			node->setScale({1,1,1});

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete rshader;
	delete node;
	delete modelEntity;
}

// Carga de texturas, aún en proceso
void loading_textures_soil_test(void) {
    GLFWwindow* window = hrn::initializeWindow();

	RShader* rshader = new RShader();
	GLuint programID = rshader->loadShaders (
            "src/shaders/test_texture_vertex.glsl"
        ,   "src/shaders/test_texture_fragment.glsl" 
    );

	float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	// load and create a texture 
    // -------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//SOIL_

	int width = 0, height = 0, channels = 0;
	Image imagedata = SOIL_load_image("assets/plantilla.png", &width, &height, 0, SOIL_LOAD_RGB);

	if(imagedata) {
		LOG("SUCCESSFULLY LOADED IMAGE!!!!");
		LOG("width: " << width);
		LOG("height: " << height);
		LOG("channels: " << channels);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
        glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		LOG("[ERROR]:: IMAGE COULDN'T LOAD");
	}

	glUseProgram(programID);
    glUniform1i(glGetUniformLocation(programID, "texture1"), 0);

	do{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render container
		glUseProgram(programID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
			&& glfwWindowShouldClose(window) == 0 );

	SOIL_free_image_data(imagedata);

	// optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
	glfwTerminate();
	delete rshader;
	// delete node;
}

// Carga una isosphere (desde assets/) y la muestra con un color de shader básico
void loading_models_learnopengl_test(void) {
    GLFWwindow* window = hrn::initializeWindow();

	RShader* rshader = new RShader();
	GLuint programID = rshader->loadShaders (
            "src/shaders/vertex.glsl"
        ,   "src/shaders/fragment.glsl" 
    );

	LOG("Shader program:" << programID);

	Node* node = new Node();
	EModel* modelEntity = new EModel();
	modelEntity->setProgramID(programID);
	modelEntity->loadFromFile("assets/missile-launcher.obj");
	// modelEntity->loadFromFile("assets/cube.obj");
	// modelEntity->loadFromFile("assets/icosphere.obj");
	// modelEntity->loadFromFile("assets/learnopengl/backpack/backpack.obj");
	
	node->setEntity(modelEntity);

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glUseProgram(programID); // Debería de seguir en emodel, pero sacado de ahi por estar en experimental
		// glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

		node->traverse(MVP);

		glfwSwapBuffers(window);
		glfwPollEvents();

		node->rotate({0.f,0.01f,0.f});

		if(glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS)
			node->rotate({0.01f,0.f,0.f});
		if(glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS)
			node->rotate({0.f,0.01f,0.f});
		if(glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS)
			node->rotate({0.f,0.f,0.01f});
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	delete rshader;
	delete node;
	delete modelEntity;
}

void cube_with_textures(void) {
    GLFWwindow* window = hrn::initializeWindow();

	RShader* rshader = new RShader();
	// GLuint programID = rshader->loadShaders (
    //         "src/shaders/vertex.glsl"
    //     ,   "src/shaders/fragment.glsl" 
    // );

	GLuint programID = rshader->loadShaders (
            "src/shaders/1.model_loading.vs"
        ,   "src/shaders/1.model_loading.fs" 
    );

	LOG("Shader program:" << programID);

	Node* node = new Node();
	// EModel* modelEntity = new EModel("assets/missile-launcher.obj");
	// EModel* modelEntity = new EModel("assets/pruebastexturas/cubo_substance.obj");
	// EModel* modelEntity = new EModel("assets/pruebastexturas/cubo_imagen.obj");
	EModel* modelEntity = new EModel("assets/pruebastexturas/cube_hardbytes.obj");
	// EModel* modelEntity = new EModel("assets/learnopengl/backpack/backpack.obj"); // DONT 

	
	modelEntity->setProgramID(programID);

	// modelEntity->loadFromFile("assets/pruebastexturas/cubo_imagen.obj");

	// modelEntity->loadFromFile("assets/missile-launcher.obj");
	// modelEntity->loadFromFile("assets/cube.obj");
	// modelEntity->loadFromFile("assets/icosphere.obj");
	// modelEntity->loadFromFile("assets/learnopengl/backpack/backpack.obj");
	// modelEntity->loadFromFile("assets/pruebastexturas/cubo_substance.obj");
	
	node->setEntity(modelEntity);

	// // view/projection transformations
	// glm::mat4 projection = Projection;
	// glm::mat4 view = View;
	// // render the loaded model
	// glm::mat4 model = glm::mat4(1.0f);
	// model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	// model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	
	// glm::mat4 mvp = projection * view * model;

	// glUseProgram(programID); // Debería de seguir en emodel, pero sacado de ahi por estar en experimental

	// // glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, &projection[0][0]);
	// // glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, &view[0][0]);
	// // glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &model[0][0]);
	// glUniformMatrix4fv(glGetUniformLocation(programID, "mvp"), 1, GL_FALSE, &mvp[0][0]);

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		node->traverse(MVP);

		glfwSwapBuffers(window);
		glfwPollEvents();

		node->rotate({0.0f,0.5f,0.f});

		if(glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS)
			node->setRotation({0,0,0});
		if(glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS)
			node->setRotation({0,90,0});
		if(glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS)
			node->setRotation({0,180,0});
		if(glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS)
			node->setRotation({0,270,0});
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	delete rshader;
	delete node;
	delete modelEntity;

	ResourceManager::freeAllResources();
} 

// READ WARNING
void resourcemanager_test(void) {
	// ADVERTENCIA: cargas los recursos, por ende los bindea con opengl
	// Si no hemos creado la ventana y demás (como en este caso)
	// va a explotar, la prueba ya está integrada pero se puede ver mejor con
	// cube_with_textures()

	// auto res1 = ResourceManager::getResource_t<RMesh>("assets/pruebastexturas/cubo_imagen.obj");
	// auto res2 = ResourceManager::getResource_t<RMesh>(res1->getName());

	// LOG(res1->getName());
	// LOG(res2->getName());
}

int main(void) {
    // tree_test();
    // cube_test();
	// loading_models_assimp_test();
	// loading_textures_soil_test();

	// loading_models_learnopengl_test();

	cube_with_textures();	// in progress
	
	// resourcemanager_test();
	
	return 0;
}
