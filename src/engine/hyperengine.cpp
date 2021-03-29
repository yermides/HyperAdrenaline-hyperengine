#include "hyperengine.hpp"

namespace hyper {

HyperEngine::HyperEngine(bool const init)
{
	if(init)
		this->initialize();
}

HyperEngine::~HyperEngine()
{
	m_shaders.clear();
	Node::deleteBranch(m_rootnode);
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    gui::DestroyContext();
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

	// glfwSwapInterval(1);	// VSync

	// Callbacks, guardar el puntero a window porque es necesario modificar dónde apunta
	// GLFWwindow* currentwindow = m_window;

	glfwSetWindowUserPointer( m_window, this );
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
		HyperEngine* engine = static_cast<HyperEngine*>( glfwGetWindowUserPointer( window ) );
		engine->setKeyState(key, action);
	});

	// glfwSetWindowUserPointer( m_window, currentwindow );


	// Initialize ImGUI
	IMGUI_CHECKVERSION();
    gui::CreateContext();
    // ImGuiIO& io = gui::GetIO(); (void)io;
    m_io = &gui::GetIO(); (void)m_io;
    gui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

	// Load shaders here
	m_shaders[OpenGLShader::SHADER_DEFAULT] = ResourceManager::getResource_t<RShader>(SHADER_DEFAULT_PATH);
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
HyperEngine::clearTree(void)
{
	// Todo:: revisar implementación
	Node::deleteBranchChilds(m_rootnode);
	m_cameras.clear();
	m_active_camera = engine_invalid_id;
	m_lights.clear();
	// m_active_lights.clear();
	m_viewports.clear();
	m_active_viewport = engine_invalid_id;

	nextCameraID      = 0;
    nextLightID       = 0;
    nextViewportID    = 0;
}


void 
HyperEngine::clearScreen(void) const
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void 
HyperEngine::beginRender(void)
{ 
	this->clearScreen();
	this->resetKeyStates();

	// Imgui

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	gui::NewFrame();
}

void 
HyperEngine::drawScene(void) const
{
	// Si no hay cámara activa, no renderizar la escena
	if(m_active_camera == engine_invalid_id)
		return;

	auto camnode = this->m_cameras[m_active_camera];
	auto camentity = static_cast<ECamera*>(camnode->getEntity());
	auto camerashader = camentity->getShader();

	// TODO:: luces
	for(auto light : m_lights) {
		// INFOLOG("TENGO LUSES!!!")
		auto lightmatrix 	= light->getMatrixTransform();
		auto lightshader 	= light->getEntity()->getShader();
		auto lightPos 		= light->getTranslation();
		auto viewPos		= camnode->getTranslation();
		auto lightColor 	= glm::vec3(1,1,1); 		// Color blanco

		// auto lightmatrix = light.m_node->getMatrixTransform();
		// auto lightshader = light.m_node->getEntity()->getShader();

		// esto debería de pasarse en Emodel
		// para saber por cada modelo si debe usar la luz o la textura difusa
		lightshader->bind();
		lightshader->setInt("usesLightning", 1);	
		lightshader->setVec3("lightPos", lightPos);
		lightshader->setVec3("viewPos", viewPos);
		lightshader->setVec3("lightColor", lightColor);
		
		// esto ni se llama así, TODO:: implementar bien el pasar el dato a opengl
		// lightshader->setMat4("lightmatrix", lightmatrix);
		lightshader->unbind();
	}


	// Pasar matrices a opengl 
	camerashader->bind();

	auto projection = camentity->getProjectionMatrix();
	camerashader->setMat4("projection", projection);

	auto cameraMatrix = camnode->getMatrixTransform();
	auto view = glm::inverse(cameraMatrix);
	camerashader->setMat4("view", view);

	camerashader->unbind();

	// Recorrer los nodos y actualizar sus matrices
	m_rootnode->traverse(glm::mat4{1.0f});
}

void 
HyperEngine::drawExampleWindowGUI(void)
{
	// TODO:: temporal, solo es un pequeño ejemplo de ventana
	gui::Begin("Ventana temporal - HyperEngine::drawExampleWindowGUI()");
	gui::Button("Hola mundo!");
	gui::End();
}

void 
HyperEngine::endRender(void) const
{ 
	// Render gui

	gui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(gui::GetDrawData());
	
	// End render
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
	// CUIDADO, esto es un emplace al final, mientras que el setActive literalmente busca posición exacta
	// TODO:: retocar comportamiento conjunto
	// auto lightdata = new LightData(light, true);
	// m_lights.push_back(light, true);

	m_lights.push_back(light);
	m_active_lights.push_back(true);

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
	// m_lights[lightID].m_active = isActive;
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
HyperEngine::getKeySinglePress(int const key) noexcept
{
	return isWindowActive() && m_keystates[key] == GLFW_PRESS;
}

bool const 
HyperEngine::getKeyContinuousPress(int const key) noexcept
{
	return isWindowActive() && glfwGetKey(m_window, key) == GLFW_PRESS; 
}

bool const 
HyperEngine::getKeyKeyboardPress(int const key) noexcept
{
	return isWindowActive() && (m_keystates[key] == GLFW_REPEAT || m_keystates[key] == GLFW_PRESS); 
}

bool const 
HyperEngine::getKeyRelease(int const key) noexcept
{
	return isWindowActive() && m_keystates[key] == GLFW_RELEASE;
}

bool const 
HyperEngine::isTreeEmpty(void)
{
	if(!m_rootnode) return true;

	return m_rootnode->getChildNumber() == 0;
}

void 
HyperEngine::setWindowTitle(std::string const& name)
{
	if(!isWindowActive()) return;

	glfwSetWindowTitle(m_window, name.c_str());
}

void 
HyperEngine::setWindowIcon(std::string const& path, int const width, int const height)
{
	// Por defecto son 32x32, pero podría aceptar 16x16 o 48x48
	if(!isWindowActive()) return;

	auto texture 	= ResourceManager::getResource_t<RTexture>(path);
	auto data 		= texture->getRawData();
	auto* icon 		= new GLFWimage{width, height, data};
	glfwSetWindowIcon(m_window, 1, icon);
}

void 
HyperEngine::setWindowClearColor(float const r, float const g, float const b, float const a)
{
	glClearColor(r,g,b,a);
}

void 
HyperEngine::setWindowActive(bool const value)
{
	if(!isWindowActive()) return;

	glfwSetWindowShouldClose(m_window, !value);
}

void 
HyperEngine::setCursorVisibility(bool const value)
{
	// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );

	// Mal planteado, ahora lo deshabilita
	glfwSetInputMode(m_window, GLFW_CURSOR, value ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED );
}

void 
HyperEngine::setCursorPosition(double const x, double const y)
{
	// Por defecto va al centro, valores a pasar entre 0 y 1 (normalizados)
	// No funciona con el cursor diabled, usar callback
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	glfwSetCursorPos(m_window, x * w , y * h);
}

void 
HyperEngine::enableZBuffer(void)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
}

void 
HyperEngine::disableZBuffer(void)
{
	glDisable(GL_DEPTH_TEST);
}

// Funciones privadas

void 
HyperEngine::setKeyState(int const key, int const action)
{
	m_keystates[key] = action;
}

void 
HyperEngine::resetKeyStates(void)
{
	// Para cada tecla reconocida en el mapa, setea a invalid (Estados: -1 invalid, 0 release, 1 press, 2 hold)
	for(auto iter = m_keystates.begin(); iter != m_keystates.end(); ++iter )
		iter->second = -1;
}

}
