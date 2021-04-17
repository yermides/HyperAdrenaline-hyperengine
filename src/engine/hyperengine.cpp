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
	this->initializeGraphics();
	this->initializePhysics();
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
HyperEngine::drawScene(void)
{
	// Si no hay cámara activa, no renderizar la escena
	if(m_active_camera == engine_invalid_id)
		return;

	auto camnode = this->m_cameras[m_active_camera];
	auto camentity = static_cast<ECamera*>(camnode->getEntity());
	auto camerashader = camentity->getShader();

	// TODO:: luces
	for(auto light : m_lights) {
		auto lightshader 	= light->getEntity()->getShader();
		auto lightPos 		= light->getTranslation();
		auto viewPos		= camnode->getTranslation();
		auto lightColor 	= glm::vec3(1,1,1); 		// Color blanco

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

	// Si quiero pintar el debug de las físicas, lo pinto
	if(m_useDebugDrawer)
		this->drawDebugPhysics(view, projection);

	// Ahora, pintar skybox si hay
	if(!m_skybox) return;

	auto skyboxEntity = static_cast<ESkybox*>(m_skybox->getEntity());
	auto skyboxShader = skyboxEntity->getShader();

	if(!skyboxShader) return;

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	skyboxShader->bind();
	view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxEntity->m_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxEntity->m_textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
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
	Viewport v{x, y, width, height};
	m_viewports.push_back(std::move(v));
	return nextViewportID++;
}

int 
HyperEngine::registerSkybox(Node* const skybox)
{
	m_skybox = skybox;
	return 0;
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

void 
HyperEngine::setActiveSkybox(Node* const node)
{
	m_skybox = node;
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

glm::dvec2
HyperEngine::getMousePositionAbsolute(void) const noexcept
{
	glm::dvec2 pos;
	glfwGetCursorPos(m_window, &pos.x, &pos.y);
	return pos;
}

void 
HyperEngine::setMousePositionAbsolute(float x, float y)
{
	auto size = this->getWindowSize();
	x = std::clamp((int)x, 0, size.x);
	y = std::clamp((int)y, 0, size.y);
	glfwSetCursorPos(m_window, x, y);
}

glm::dvec2 
HyperEngine::getMousePositionRelative(void) const noexcept
{
	// Devolver en coordenadas discretas, de 0 a 1

	// glm::dvec2 pos = this->getMousePositionAbsolute();

	// int w, h;
	// glfwGetWindowSize(m_window, &w, &h);
	// glfwSetCursorPos(m_window, x * w , y * h);

	return glm::dvec2(0,0);
}

void 
HyperEngine::setMousePositionRelative(float x, float y)
{

}

void 
HyperEngine::setMousePositionToCenter(void)
{
	auto size = this->getWindowSize();
	glfwSetCursorPos(m_window, size.x / 2.0, size.y / 2.0);
}

void 
HyperEngine::setCursorVisibility(bool const value)
{
	// Modos: GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED
	// Cuando se deshabilita el mouse, aún actúa de manera oculta pero al volver no recuerda la posición de moverse en oculto
	// de ahí el reposicionamiento

	auto mode = glfwGetInputMode(m_window, GLFW_CURSOR);

	if(mode == GLFW_CURSOR_NORMAL && !value)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if(mode == GLFW_CURSOR_DISABLED && value)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->setMousePositionToCenter();
	}
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

bool const 
HyperEngine::isTreeEmpty(void)
{
	if(!m_rootnode) return true;

	return m_rootnode->getChildNumber() == 0;
}

glm::ivec2 
HyperEngine::getWindowSize(void)
{
	glm::ivec2 size;
	glfwGetWindowSize(m_window, &size.x, &size.y);
	return size;
}

glm::ivec2 
HyperEngine::getWindowSizeCenter(void)
{
	auto size = getWindowSize();
	size.x /= 2.0;
	size.y /= 2.0;
	return size;
}


void 
HyperEngine::setWindowSize(int const width, int const height)
{
	glfwSetWindowSize(m_window, width, height);
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
	auto icon 		= new GLFWimage{width, height, data};
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
HyperEngine::enableZBuffer(int const method)
{
	glEnable(GL_DEPTH_TEST);

	// Methods: GL_ALWAYS, GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL
	glDepthFunc(method);
}

void 
HyperEngine::disableZBuffer(void)
{
	glDisable(GL_DEPTH_TEST);
}

void 
HyperEngine::enableCulling(int const method, int const faceselection)
{
	glEnable(GL_CULL_FACE);

	// Methods: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
	glCullFace(method);

	// Face selection: GL_CW, GL_CCW (clockwise or counter-clockwise)
	glFrontFace(faceselection);
}

void 
HyperEngine::disableCulling(void)
{
	glDisable(GL_CULL_FACE);
}

void 
HyperEngine::updatePhysics(float const deltatime)
{
	m_world->stepSimulation(deltatime);
}

void 
HyperEngine::createRigidbody(Node * const node)
{
	static int cinematic_mass = 0;

	auto prop = node->getPhysicProperties();
	
	if(prop) return;

	// Setup de variables necesarias para la creación del rigidbody
	btTransform transform;
	transform.setIdentity();
	auto trans = node->getTranslation();
	transform.setOrigin( btVector3(trans.x, trans.y, trans.z) );
	btDefaultMotionState *motionState = new btDefaultMotionState(transform);
	// Definiendo las dimensiones de la bounding box, hardcoded as fck boiii // TODO:: parametrizar
	btCollisionShape *shape = new btBoxShape( btVector3(1,1,1) );

	btVector3 localInertia;
	shape->calculateLocalInertia(cinematic_mass, localInertia);

	// Creando finalmente el rigidbody y seteando parámetros
	btRigidBody *body = new btRigidBody(cinematic_mass, motionState, shape, localInertia);

	using CO = btCollisionObject;
	// TODO:: parametrizar, solo son cinemáticos de momento
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_KINEMATIC_OBJECT); 
	body->setActivationState(DISABLE_DEACTIVATION);

	// Pasárselo al nodo
	Node::PhysicProperties* properties = new Node::PhysicProperties;
	properties->body = body;
	node->setPhysicProperties(properties);

	// Y finalmente al mundo
	m_world->addRigidBody(body);

	// Debug, TODO:: borrar
	btTransform nodetransform;
	body->getMotionState()->getWorldTransform(nodetransform);
	auto pos = nodetransform.getOrigin();
	INFOLOG("Posición del nodo: " << VAR(pos.x()) << VAR(pos.y()) << VAR(pos.z()) );
}

void 
HyperEngine::createRigidBodyConvexHull(Node * const node)
{
	static int cinematic_mass = 0;

	auto prop = node->getPhysicProperties();
	
	if(prop) return;

	// Setup de variables necesarias para la creación del rigidbody
	btTransform transform;
	transform.setIdentity();
	auto trans = node->getTranslation();
	transform.setOrigin( btVector3(trans.x, trans.y, trans.z) );
	btDefaultMotionState *motionState = new btDefaultMotionState(transform);
	// Definiendo las dimensiones de la bounding box, hardcoded as fck boiii // TODO:: parametrizar
	auto convexhull = new btConvexHullShape;

	auto model = static_cast<EModel*>(node->getEntity());
	auto vertices = model->getVertexPositions();

	for(auto i {0}; i< vertices.size(); i=i+3) {
		convexhull->addPoint(btVector3(vertices[i], vertices[i+1], vertices[i+2]));
	}

	btCollisionShape *shape = convexhull;


	btVector3 localInertia;
	shape->calculateLocalInertia(cinematic_mass, localInertia);

	// Creando finalmente el rigidbody y seteando parámetros
	btRigidBody *body = new btRigidBody(cinematic_mass, motionState, shape, localInertia);

	using CO = btCollisionObject;
	// TODO:: parametrizar, solo son cinemáticos de momento
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_KINEMATIC_OBJECT); 
	body->setActivationState(DISABLE_DEACTIVATION);

	// Pasárselo al nodo
	Node::PhysicProperties* properties = new Node::PhysicProperties;
	properties->body = body;
	node->setPhysicProperties(properties);

	// Y finalmente al mundo
	m_world->addRigidBody(body);

	// Debug, TODO:: borrar
	btTransform nodetransform;
	body->getMotionState()->getWorldTransform(nodetransform);
	auto pos = nodetransform.getOrigin();
	INFOLOG("Posición del nodo: " << VAR(pos.x()) << VAR(pos.y()) << VAR(pos.z()) );
}

void
HyperEngine::drawDebugPhysics(glm::mat4 const& view, glm::mat4 const& projection)
{
	// INFOLOG("Dibujando fisicas (debug)");

	auto debugdrawshader = m_shaders[OpenGLShader::SHADER_DEBUGDRAWER];

	debugdrawshader->bind();
	m_debugDrawer->setMatrices(view, projection, debugdrawshader);
	m_world->debugDrawWorld();
}

void 
HyperEngine::enableDebugDraw(void)
{
	m_useDebugDrawer = true;
}

void 
HyperEngine::disableDebugDraw(void)
{
	m_useDebugDrawer = false;
}

DebugDrawer* const 
HyperEngine::getDebugDrawer(void)
{
	return m_debugDrawer;
}

void 
HyperEngine::setDebugDrawer(DebugDrawer* debugDrawer)
{
	m_debugDrawer = debugDrawer;
	m_world->setDebugDrawer(m_debugDrawer);
}

// Funciones privadas

void 
HyperEngine::initializeGraphics(void)
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

	// OpenGL new compatibility (glfw)
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	gui::CreateContext();
	// ImGuiIO& io = gui::GetIO(); (void)io;
	m_io = &gui::GetIO(); (void)m_io;
	gui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Load shaders here
	m_shaders[OpenGLShader::SHADER_DEFAULT] 		= ResourceManager::getResource_t<RShader>(SHADER_DEFAULT_PATH);
	m_shaders[OpenGLShader::SHADER_SKYBOX] 			= ResourceManager::getResource_t<RShader>(SHADER_SKYBOX_PATH);
	m_shaders[OpenGLShader::SHADER_DEBUGDRAWER] 	= ResourceManager::getResource_t<RShader>(SHADER_DEBUGDRAWER_PATH);
	
}

void 
HyperEngine::initializePhysics(void)
{
	btCollisionConfiguration *collisionConfiguration 	=     new btDefaultCollisionConfiguration();
	btBroadphaseInterface *broadPhase 					=     new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	btDispatcher *dispatcher 							=     new btCollisionDispatcher(collisionConfiguration);
	btConstraintSolver *solver 							=     new btSequentialImpulseConstraintSolver();

	// World seteado
	m_world 											=     new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);

	// setear su debug drawer y opciones por defecto
	m_debugDrawer = new DebugDrawer;
	m_debugDrawer->setDebugMode(DebugDrawer::DBG_DrawWireframe | DebugDrawer::DBG_DrawAabb);
	m_world->setDebugDrawer(m_debugDrawer);
}

void 
HyperEngine::setKeyState(int const key, int const action)
{
	m_keystates[key] = action;
}

void 
HyperEngine::resetKeyStates(void)
{
	// Para cada tecla reconocida en el mapa, setea a invalid (Estados: -1 invalid, 0 release, 1 press, 2 hold)
	for(auto iter = m_keystates.begin(); iter != m_keystates.end(); ++iter)
		iter->second = -1;
}

}
