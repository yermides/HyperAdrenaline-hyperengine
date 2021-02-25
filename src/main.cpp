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

	// RShader* rshader = new RShader();
	// GLuint programID = rshader->loadShaders (
    //         "src/shaders/vertex.glsl"
    //     ,   "src/shaders/fragment.glsl" 
    // );
	// Node* node = new Node();

	int width = 0, height = 0, channels = 0;
	Image image = SOIL_load_image("assets/plantilla.png", &width, &height, &channels, 4);

	if(image) {
		LOG("SUCCESSFULLY LOADED IMAGE!!!!");
	} else {
		LOG("[ERROR]:: IMAGE COULDN'T LOAD");
	}

	SOIL_free_image_data(image);

	// for (unsigned int i = 0;  i < pathcount;  i++)
    // {
	// 	   int width = 0, height = 0, channels = 0;
    //     unsigned char* imagedata = SOIL_load_image(paths[i], &width, &height, &channels, 4);
    //     if (imagedata)
    //     {
    //         glBindTexture(GL_TEXTURE_2D, textures[Texture_Vertices]);
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);
    //         glBindTexture(GL_TEXTURE_2D, 0);

    //         glfwSetWindowSize(window, 800, (800 * height) / width);

    //         cout << "loading image: \t"<< paths[i] << ": \t" << width << " x " << height << endl;
    //         break;
    //     }
    //     else
    //         cout << "cant load image: \t"<< paths[i] << endl;
    //     SOIL_free_image_data(imagedata);
    // }

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// node->traverse(MVP);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
			&& glfwWindowShouldClose(window) == 0 );

	glfwTerminate();
	// delete rshader;
	// delete node;
}

int main(void) {
    // tree_test();
    // cube_test();
	// loading_models_assimp_test();
	loading_textures_soil_test();

	return 0;
}

