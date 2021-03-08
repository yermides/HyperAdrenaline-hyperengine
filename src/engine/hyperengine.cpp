#include "hyperengine.hpp"

HyperEngine::HyperEngine()
{
}

HyperEngine::~HyperEngine()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void 
HyperEngine::initialize()
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

	return node;
}

void 
HyperEngine::drawScene() 
{
	// TODO:: Temporal, empezar con la matriz identidad
	m_rootnode->traverse(glm::mat4{1.0f});
}
