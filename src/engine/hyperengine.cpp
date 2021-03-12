#include "hyperengine.hpp"

HyperEngine::HyperEngine(bool const init)
{
	if(init)
		this->initialize();
}

HyperEngine::~HyperEngine()
{
	Node::deleteBranch(m_rootnode);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void 
HyperEngine::initialize(void)
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		throw new std::runtime_error("Failed to initialize GLFW");
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);					// antialiasing x4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	m_window = glfwCreateWindow( 1366, 768, "HyperEngine Test 1", NULL, NULL);
	if( !m_window ) {
		throw new std::runtime_error("Failed to open GLFW window");
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(m_window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		throw new std::runtime_error("Failed to initialize GLEW");
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Default clear color = Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glfwSwapInterval(1);	// VSync
}

Node* 
HyperEngine::createNode(Node* const parent, glm::vec3 const& trans, glm::vec3 const& rot, glm::vec3 const& scale)
{
	Node* node { new Node };

	if(!parent) {
		m_rootnode->addChild(node);
	} else {
		parent->addChild(node);
	}

	node->setTranslation(trans);
	node->setRotation(rot);
	node->setScale(scale);

	INFOLOG("Memory address of rootnode is " << VAR(m_rootnode));
	INFOLOG("Memory address of new created rootnode child is " << VAR(node));

	return node;
}

void 
HyperEngine::clearScreen(void) const
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void 
HyperEngine::beginRender(void) const
{ 
	this->clearScreen();
}

void 
HyperEngine::drawScene(void) const
{
	// Si no hay cámara activa, no renderizar la escena
	if(m_active_camera == engine_invalid_id)
		return;

	// TODO:: luces
	// for(auto light : m_lights) {
	// 	auto lightmatrix = light->getMatrixTransform();
	// 	auto shaderID = light->getEntity()->getProgramID();
	// 	// pasarle el dato a opengl, puede ser en la función draw() de la entidad light
	// 	// glUniform3f(glGetUniformLocation(shaderID, "propiedad", x, y, z);
	// }

	auto activeCamNode = this->m_cameras[m_active_camera];
	auto camentity = static_cast<ECamera*>(activeCamNode->getEntity());
	auto shaderID = camentity->getProgramID();

	// Pasar matrices a opengl 
	glUseProgram(shaderID);

	auto projection = camentity->getProjectionMatrix();

    INFOLOG("Se encuentra la propiedad projection: " << VAR(glGetUniformLocation(shaderID, "projection")));
    glUniformMatrix4fv(
        glGetUniformLocation(shaderID, "projection")
        , 1
        , GL_FALSE
        , &projection[0][0]
    );

	auto cameraMatrix = activeCamNode->getMatrixTransform();
	auto view = glm::inverse(cameraMatrix);

    INFOLOG("Se encuentra la propiedad view: " << VAR(glGetUniformLocation(shaderID, "view")));

	glUniformMatrix4fv(
        glGetUniformLocation(shaderID, "view")
        , 1
        , GL_FALSE
        , &view[0][0]
    );

	glUseProgram(0);

	m_rootnode->traverse(glm::mat4{1.0f});
}

void 
HyperEngine::endRender(void) const
{ 
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

int 
HyperEngine::registerCamera(Node* const camera) 
{
	m_cameras.push_back(camera);

	if(m_active_camera == engine_invalid_id)
		m_active_camera = nextCameraID;

	return nextCameraID++;
}

int 
HyperEngine::registerLight(Node* const light) 
{
	m_lights.push_back(light);
	return nextLightID++;
}

int 
HyperEngine::registerViewport(int x, int y, int height, int width) 
{
	Viewport viewport;
	viewport.x = x; viewport.y = y;
	viewport.height = height; viewport.width = width;
	m_viewports.push_back(std::move(viewport));
	return nextViewportID++;
}

void 
HyperEngine::setActiveCamera(int const camID) 
{
	m_active_camera = camID;
}

void 
HyperEngine::setActiveLight(int const lightID, bool const isActive) 
{
	m_active_lights[lightID] = isActive;
}

void 
HyperEngine::setActiveViewport(int const viewportID) 
{
	m_active_viewport = viewportID;
}

bool const 
HyperEngine::isWindowActive(void) const noexcept
{ 
	if(!m_window || glfwWindowShouldClose(m_window))
		return false;

	return true;
}

GLFWwindow* 
HyperEngine::getWindow(void) const noexcept
{ 
	return m_window; 
}

bool const 
HyperEngine::isKeyPressed(int const keycode) const noexcept
{ 
	if(!isWindowActive())
		return false;

	return glfwGetKey(m_window, keycode) == GLFW_PRESS; 
}
