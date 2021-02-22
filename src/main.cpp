/*
    This file exists only for testing
    it'll be removed once the engine
    is compiled into an .a file
*/
#include <main.hpp>

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

int cube_test(void) {
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

	// This matrix belongs to the ECamera
	// Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(
		glm::radians(45.0f)
		, 16.0f / 9.0f
		, 0.1f
		, 100.0f
	);

	// This matrix belongs to the Node that contains ECamera (and is inverted)
	// Camera matrix
	glm::mat4 View  = glm::lookAt(
						glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
						glm::vec3(0,0,0), // and looks at the origin
						glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
					);

	// This matrix belongs to the Node that contains the EMesh
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

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

	return 0;
}

int main(void) {
    // tree_test();
    cube_test();
}
