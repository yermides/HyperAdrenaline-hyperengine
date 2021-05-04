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
	m_keystates.clear();
	Node::deleteBranch(m_rootnode);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	gui::DestroyContext();
	glfwDestroyWindow(m_window);
	glfwTerminate();
	m_collisionPairs.clear();
	delete m_world;
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
	// Clear screen and reset keys
	this->clearScreen();
	this->resetKeyStates();
	this->resetMouseKeyStates();
	this->resetMouseWheelStatus();

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
	int count {0};			// Para no cambiar el range-for loop
	int numDirLights {0};
	int numPointLights {0};
	int numSpotLights {0};

	for(auto light : m_lights) {

		auto lightentity = light->getEntityAsLight();

		// Cuidado con el range equis dé
		if(!m_active_lights.at(count)) 
		{
			++count;
			continue; 
		}
		else {
			++count;
		}

		// INFOLOG("count!" << VAR(count))

		auto lightshader 	= lightentity->getShader();
		auto lightPos 		= light->getTranslation();
		auto viewPos		= camnode->getTranslation();
		auto lightColor 	= glm::vec3(1,1,1); 		// Color blanco

		switch(lightentity->getType())
		{
			case LightType::Point:
			{
				// INFOLOG("Luz puntual!");

				// esto debería de pasarse en Emodel
				// para saber por cada modelo si debe usar la luz o la textura difusa
				lightshader->bind();

				lightshader->setInt("numPointLights", numPointLights + 1);
				lightshader->setVec3("viewPos", viewPos);

				// Estos ya no se usan, pero por si acaso quiero volver al shader de materiales
				// lightshader->setInt("usesLightning", 1);
				// lightshader->setVec3("lightColor", lightColor);

				lightshader->setVec3("pointLights["+ std::to_string(numPointLights) +"].position", lightPos);
				lightshader->setVec3("pointLights["+ std::to_string(numPointLights) +"].ambient", lightentity->getIntensity().ambient);
				lightshader->setVec3("pointLights["+ std::to_string(numPointLights) +"].diffuse", lightentity->getIntensity().diffuse);
				lightshader->setVec3("pointLights["+ std::to_string(numPointLights) +"].specular", lightentity->getIntensity().specular);
				lightshader->setFloat("pointLights["+ std::to_string(numPointLights) +"].constant", lightentity->getAttenuation().constant);
				lightshader->setFloat("pointLights["+ std::to_string(numPointLights) +"].linear", lightentity->getAttenuation().linear);
				lightshader->setFloat("pointLights["+ std::to_string(numPointLights) +"].quadratic", lightentity->getAttenuation().quadratic);
				
				// esto ni se llama así, TODO:: implementar bien el pasar el dato a opengl
				// lightshader->setMat4("lightmatrix", lightmatrix);
				lightshader->unbind();

				++numPointLights;
				break;
			}

			case LightType::Directional: 
			{
				// INFOLOG("Luz direccional!");

				lightshader->bind();

				lightshader->setInt("numDirLights", numDirLights + 1);
				lightshader->setVec3("viewPos", viewPos);

				lightshader->setVec3("dirLights["+ std::to_string(numDirLights) +"].direction", lightentity->getDirection());
				lightshader->setVec3("dirLights["+ std::to_string(numDirLights) +"].ambient", lightentity->getIntensity().ambient);
				lightshader->setVec3("dirLights["+ std::to_string(numDirLights) +"].diffuse", lightentity->getIntensity().diffuse);
				lightshader->setVec3("dirLights["+ std::to_string(numDirLights) +"].specular", lightentity->getIntensity().specular);
				
				lightshader->unbind();

				++numDirLights;
				break;
			}

			case LightType::Spot: 
			{
				// INFOLOG("Luz focal!");

				lightshader->bind();

				lightshader->setInt("numSpotLights", numSpotLights + 1);
				lightshader->setVec3("viewPos", viewPos);

				lightshader->setVec3("spotLights["+ std::to_string(numSpotLights) +"].position", lightPos);
				lightshader->setVec3("spotLights["+ std::to_string(numSpotLights) +"].direction", lightentity->getDirection());
				lightshader->setVec3("spotLights["+ std::to_string(numSpotLights) +"].ambient", lightentity->getIntensity().ambient);
				lightshader->setVec3("spotLights["+ std::to_string(numSpotLights) +"].diffuse", lightentity->getIntensity().diffuse);
				lightshader->setVec3("spotLights["+ std::to_string(numSpotLights) +"].specular", lightentity->getIntensity().specular);
				lightshader->setFloat("spotLights["+ std::to_string(numSpotLights) +"].constant", lightentity->getAttenuation().constant);
				lightshader->setFloat("spotLights["+ std::to_string(numSpotLights) +"].linear", lightentity->getAttenuation().linear);
				lightshader->setFloat("spotLights["+ std::to_string(numSpotLights) +"].quadratic", lightentity->getAttenuation().quadratic);
				lightshader->setFloat("spotLights["+ std::to_string(numSpotLights) +"].cutOff", glm::cos(glm::radians(lightentity->getAperture().innerCutoff)));
				lightshader->setFloat("spotLights["+ std::to_string(numSpotLights) +"].outerCutOff", glm::cos(glm::radians(lightentity->getAperture().outerCutoff)));
				
				lightshader->unbind();

				++numSpotLights;
				break;
			}

			default: 
				break;
		}
	}

	// Pasar matrices a opengl 
	camerashader->bind();

	auto projection = camentity->getProjectionMatrix();
	camerashader->setMat4("projection", projection);


	// Ya no es así porque setCameraTarget machaca toda la transformación necesariamente
	// auto cameraMatrix = camnode->getMatrixTransform();
	// auto view = glm::inverse(cameraMatrix);

	auto cameraMatrix = camentity->getViewMatrix();
	auto view = cameraMatrix;

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

bool 
HyperEngine::isSkyboxActive(void) const noexcept
{
	if(!m_skybox) 	return false;
	else 			return true;
}

void 
HyperEngine::deleteSkybox(void)
{
	if(!m_skybox) return;

	m_rootnode->removeChild(m_skybox);
	delete m_skybox;
	m_skybox = nullptr;
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

/*****/
bool const 
HyperEngine::getMouseKeySinglePress(int const key) noexcept
{
	return isWindowActive() && m_mousekeystates[key] == GLFW_PRESS;
}

bool const 
HyperEngine::getMouseKeyContinuousPress(int const key) noexcept
{
	return isWindowActive() && glfwGetMouseButton(m_window, key) == GLFW_PRESS; 
}

bool const 
HyperEngine::getMouseKeyKeyboardPress(int const key) noexcept
{
	// No funciona, hace lo mismo que el single press
	return isWindowActive() && (m_mousekeystates[key] == GLFW_REPEAT || m_mousekeystates[key] == GLFW_PRESS); 
}

bool const 
HyperEngine::getMouseKeyRelease(int const key) noexcept
{
	return isWindowActive() && m_mousekeystates[key] == GLFW_RELEASE;
}

MouseWheelStatus const& 
HyperEngine::getMouseWheelStatus(void)
{
	return m_mouseWheelStatus;
}

/*****/

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
HyperEngine::createPhysicPropertiesRigidBody(
	Node* const node
, 	btCollisionShape* pShape
, 	float mass
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	// Translación y rotación iguales a las del nodo
	auto nodetrans = util::glmVec3TobtVec3(node->getTranslation());
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(nodetrans);

	auto noderot = node->getRotation();
	btQuaternion btquatx( {1.0,0.0,0.0}, glm::radians( noderot.x ) );
	btQuaternion btquaty( {0.0,1.0,0.0}, glm::radians( noderot.y ) );
	btQuaternion btquatz( {0.0,0.0,1.0}, glm::radians( noderot.z ) );
	btQuaternion btquat = btquatx * btquaty * btquatz;
	transform.setRotation(btquat);

	// Construcción del rigid body
	auto collisionShape = pShape;
	auto motionState = new OpenGLMotionState(transform);
	btVector3 localInertia(0,0,0);

	if (mass != 0.0f)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, motionState, collisionShape, localInertia);
	auto rigidBody = new btRigidBody(cInfo);
	rigidBody->setUserPointer( static_cast<void*>(node) );	// Contiene al nodo para facilitar el coste de la búsqueda

	// Construcción de las propiedades físicas del nodo
	auto properties = new PhysicProperties;
	properties->m_type = PhysicProperties::PhysicDatatype::RIGID_BODY;
	properties->m_data.body			= rigidBody;
	properties->m_collisionShape	= collisionShape;
	properties->m_motionState 		= motionState;
	node->setPhysicProperties(properties);

	m_world->addRigidBody(rigidBody);

	// Meter las flags del grupo de objetos colisionables y los objetos contra los que puede colisionar
	// TODO:: descomentar cuando se haga bien
	// m_world->addRigidBody(rigidBody, collisionGroupFlags, collisionMaskFlags);	
}

void 
HyperEngine::createPhysicPropertiesCollisionObject(
	Node* const node
,   btCollisionShape* pShape
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	btCollisionObject* collisionObject = new btCollisionObject;
	collisionObject->setUserPointer( static_cast<void*>(node) );
	collisionObject->setCollisionShape(pShape);

	auto nodetrans = util::glmVec3TobtVec3(node->getTranslation());
	btTransform trans;
	trans.setOrigin(nodetrans);

	auto noderot = node->getRotation();
	btQuaternion btquatx( {1.0,0.0,0.0}, glm::radians( noderot.x ) );
	btQuaternion btquaty( {0.0,1.0,0.0}, glm::radians( noderot.y ) );
	btQuaternion btquatz( {0.0,0.0,1.0}, glm::radians( noderot.z ) );
	btQuaternion btquat = btquatx * btquaty * btquatz;
	trans.setRotation(btquat);

	// Sustituto de meter el motionState inicial (directamente meter la transformación inicial)
	collisionObject->setWorldTransform(trans);

	// Meter los datos del colObj y guardarlo en node
	auto prop = new PhysicProperties;
	prop->m_type = PhysicProperties::PhysicDatatype::COLLISION_OBJECT;
	prop->m_data.collObj = collisionObject;
	node->setPhysicProperties(prop);

	// m_world->addCollisionObject(collisionObject);

	// TODO:: Para cuando se pongan las flags bien
	m_world->addCollisionObject(collisionObject, collisionGroupFlags, collisionMaskFlags);
}

void 
HyperEngine::createPhysicPropertiesStaticBody(
	Node* const node
,   btCollisionShape* pShape
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	// No afectado por las fuerzas por tener masa cero
	this->createPhysicPropertiesRigidBody(
			node
		,	pShape
		,	0
		,	collisionGroupFlags
		,	collisionMaskFlags
	);

	auto properties = node->getPhysicProperties();
	auto body 		= properties->m_data.body;

	using CO = btCollisionObject;
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_STATIC_OBJECT); 
}

void 
HyperEngine::createPhysicPropertiesKinematicBody(
	Node* const node
,   btCollisionShape* pShape
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	// No afectado por las fuerzas por tener masa cero
	this->createPhysicPropertiesRigidBody(
			node
		,	pShape
		,	0
		,	collisionGroupFlags
		,	collisionMaskFlags
	);

	auto properties = node->getPhysicProperties();
	auto body 		= properties->m_data.body;

	using CO = btCollisionObject;
	// Marcado como cinemático para que el world consulte su motionState en cada iteración (modificable por el usuario)
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_KINEMATIC_OBJECT); 
	// Marcado como que bullet no puede llegar a ignorar sus físicas por optimizaciones internas
	body->setActivationState(DISABLE_DEACTIVATION);
}

void 
HyperEngine::createPhysicPropertiesDynamicBody(
	Node* const node
,   btCollisionShape* pShape
,   float mass
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	// Masa mínima de 1
	mass = std::max(mass, 1.0f);

	// No afectado por las fuerzas por tener masa cero
	this->createPhysicPropertiesRigidBody(
			node
		,	pShape
		,	mass
		,	collisionGroupFlags
		,	collisionMaskFlags
	);

	auto properties = node->getPhysicProperties();
	auto body 		= properties->m_data.body;

	using CO = btCollisionObject;
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_DYNAMIC_OBJECT); 
}

void 
HyperEngine::createPhysicPropertiesTriangleMeshShape(
	Node* const node
,   float const mass
,   int collisionGroupFlags
,   int collisionMaskFlags
)
{
	if(!node || node->getPhysicProperties()) return;

	auto model 			= static_cast<EModel*>(node->getEntity());
	auto vertices 		= model->getVertexPositions();
	auto indices 		= model->getVertexIndices();
	auto trianglemesh	= new btTriangleMesh;

	for(uint32_t i {0}; i < vertices.size(); i+=9 )
	{
		trianglemesh->addTriangle(
				btVector3(vertices[i], vertices[i+1], vertices[i+2])
			,	btVector3(vertices[i+3], vertices[i+4], vertices[i+5])
			,	btVector3(vertices[i+6], vertices[i+7], vertices[i+8])
		);
	}

	for(uint32_t i {0}; i < indices.size(); i+=3 )
	{
		trianglemesh->addTriangleIndices(
			indices[i], indices[i+1], indices[i+2]
		);
	}

	auto shape	= new btBvhTriangleMeshShape(trianglemesh, false);

	// Realmente sería "this->createPhysicPropertiesStaticBody"
	this->createPhysicPropertiesRigidBody(
			node
		,	shape
		,	mass
		,   collisionGroupFlags
		,   collisionMaskFlags
	);
}


bool 
HyperEngine::getAABBCollisionBetweenNodes(Node* const nodeA, Node* const nodeB)
{
	if(!nodeA || !nodeB) return false;

	auto it = std::find_if(m_collisionPairs.begin(), m_collisionPairs.end(), [nodeA, nodeB](CollisionPairResult& collPair){
		return ( collPair.IDs.first == nodeA->getNameID() && collPair.IDs.second == nodeB->getNameID() ) 
			|| ( collPair.IDs.first == nodeB->getNameID() && collPair.IDs.second == nodeA->getNameID() );
	});

	if(it == m_collisionPairs.end()) return false;

	return true;
}

bool 
HyperEngine::getCollisionBetweenNodes(Node* const nodeA, Node* const nodeB, CollisionPairResult& collPairResult)
{
	if(!nodeA || !nodeB) return false;

	auto it = std::find_if(m_collisionPairs.begin(), m_collisionPairs.end(), [nodeA, nodeB](CollisionPairResult& collPair){
		return ( collPair.IDs.first == nodeA->getNameID() && collPair.IDs.second == nodeB->getNameID() ) 
			|| ( collPair.IDs.first == nodeB->getNameID() && collPair.IDs.second == nodeA->getNameID() );
	});

	if(it == m_collisionPairs.end()) return false;

	collPairResult = *it;

	return !it->points.empty();


	// if(!nodeA || !nodeB) return false;

	// auto& cp = m_collisionPairs;
	// auto it = 
	// 	std::find_if(cp.begin(), cp.end(), 
	// 		[&nodeA, &nodeB](std::pair<Node::NodeID, Node::NodeID> const& p)
	// 		{
	// 			return ( p.first == nodeA->getNameID() && p.second == nodeB->getNameID() ) 
	// 				|| ( p.first == nodeB->getNameID() && p.second == nodeA->getNameID() );
	// 		}
	// 	);

	// if(it != cp.end())
	// 	return true;
	// else 
	// 	return false;
}

// bool 
// HyperEngine::getCollisionBetweenNodes(Node* const nodeA, Node* const nodeB, PhysicContactResult& result)
// {
// 	// Patata, no funciona, no mires

// 	// m_world->contactPairTest(collObjA, collObjB, callback);
// 	// m_world->contactTest();
// 	// m_world->contactPairTest();
// }

void 
HyperEngine::deletePhysicProperties(Node* const node)
{
	if(!node) return;
	auto prop = node->getPhysicProperties();
	if(!prop) return;

	// COLLISION_OBJECT, RIGID_BODY
	m_world->removeCollisionObject(prop->m_data.collObj);
	delete prop;
	node->setPhysicProperties(nullptr);
}

// Old functions for physics

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
	// Esto desactiva totalmente el estado de colisiones, ya no colisionará con nada, WARNING!!!!!1!!!!111!
	body->setActivationState(DISABLE_DEACTIVATION);

	// Pasárselo al nodo
	PhysicProperties* properties = new PhysicProperties;
	properties->m_data.body = body;
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

	for(std::size_t i {0}; i< vertices.size(); i=i+3) {
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
	PhysicProperties* properties = new PhysicProperties;
	properties->m_data.body = body;
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
HyperEngine::createRigidBodyDynamic(Node * const node)
{
	static float dynamic_mass = 1.0f;

	// auto prop = node->getPhysicProperties();
	// if(prop) return;

	// Setup de variables necesarias para la creación del rigidbody
	btTransform transform;
	transform.setIdentity();
	// auto trans = node->getTranslation();
	transform.setOrigin( btVector3(0, 2, 0) );
	btDefaultMotionState *motionState = new btDefaultMotionState(transform);

	// Definiendo las dimensiones de la bounding box, de momento aquí es una tremenda esfera
	btCollisionShape *shape = new btSphereShape(1);

	btVector3 localInertia;
	shape->calculateLocalInertia(dynamic_mass, localInertia);

	// Creando finalmente el rigidbody y seteando parámetros
	btRigidBody *body = new btRigidBody(dynamic_mass, motionState, shape, localInertia);

	// using CO = btCollisionObject;
	// TODO:: parametrizar, solo son cinemáticos de momento
	// body->setCollisionFlags(body->getCollisionFlags() | CO::CF_KINEMATIC_OBJECT | CO::CF_DYNAMIC_OBJECT); 
	// body->setActivationState(DISABLE_DEACTIVATION);

	// Pasárselo al nodo
	// PhysicProperties* properties = new PhysicProperties;
	// properties->body = body;
	// node->setPhysicProperties(properties);

	// Y finalmente al mundo
	m_world->addRigidBody(body);
}

void 
HyperEngine::createTriangleMeshShape(Node * const node)
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

	// Definiendo la triangle mesh

	auto model 			= static_cast<EModel*>(node->getEntity());
	auto trianglemesh	= new btTriangleMesh;
	auto vertices 		= model->getVertexPositions();

	for(std::size_t i {0}; i<vertices.size(); i=i+9) {
		trianglemesh->addTriangle(
				btVector3(vertices[i], vertices[i+1], vertices[i+2])
			,	btVector3(vertices[i+3], vertices[i+4], vertices[i+5])
			,	btVector3(vertices[i+6], vertices[i+7], vertices[i+8])
		);
	}

	auto shape	= new btBvhTriangleMeshShape(trianglemesh, false);

	btVector3 localInertia;
	shape->calculateLocalInertia(cinematic_mass, localInertia);

	// Creando finalmente el rigidbody y seteando parámetros
	btRigidBody *body = new btRigidBody(cinematic_mass, motionState, shape, localInertia);

	using CO = btCollisionObject;
	// TODO:: parametrizar, solo son cinemáticos de momento
	body->setCollisionFlags(body->getCollisionFlags() | CO::CF_KINEMATIC_OBJECT); 
	body->setActivationState(DISABLE_DEACTIVATION);

	// Pasárselo al nodo
	PhysicProperties* properties = new PhysicProperties;
	properties->m_data.body = body;
	node->setPhysicProperties(properties);

	// Y finalmente al mundo
	m_world->addRigidBody(body);
}

bool 
HyperEngine::throwRaycast(
	const btVector3 &startPosition
, 	const btVector3 &direction
, 	RayResult &output
, 	int collisionGroupMask
, 	int collisionFilterMask
)
{
	if (!m_world) return false;
	
	// get the picking ray from where we clicked
	btVector3 rayTo = direction;
	btVector3 rayFrom = startPosition;
	
	// create our raycast callback object
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	rayCallback.m_collisionFilterGroup = collisionGroupMask;
	rayCallback.m_collisionFilterMask = collisionFilterMask;
	
	// perform the raycast
	m_world->rayTest(rayFrom, rayTo, rayCallback);

	if(m_useDebugDrawer)
		m_debugDrawer->drawLine(rayFrom, rayTo, btVector3(0,1,0));
	
	// did we hit something?
	if (rayCallback.hasHit())
	{
		// if so, get the rigid body we hit
		// TODO:: aplicar el mismo union o dejarlo como collisionObject en el result (done, pero maybe revisar)
		// auto* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		auto* pObj =  const_cast<btCollisionObject*>(rayCallback.m_collisionObject);

		// btCollisionObject* pBody =  const_cast<btCollisionObject*>(rayCallback.m_collisionObject);
		// btCollisionObject* pBody =  rayCallback.m_collisionObject;

		// if (!pBody) return false;
	
		// prevent us from picking objects like the ground plane

		// TODO:: descomentar
		// if (pBody->isStaticObject() || pBody->isKinematicObject()) 
		// 	return false;
	
		// set the result data
		output.pObj = pObj;
		output.hitPoint = rayCallback.m_hitPointWorld;
		output.node = static_cast<Node*>(pObj->getUserPointer());
		return true;
	}

	// we didn't hit anything
	return false;
}

bool 
HyperEngine::throwRaycastAllHits(
	const btVector3 &startPosition
, 	const btVector3 &direction
, 	std::vector<RayResult>& output
, 	int collisionGroupMask
, 	int collisionFilterMask
)
{
	if (!m_world) return false;
	
	btVector3 rayTo = direction;
	btVector3 rayFrom = startPosition;
	
	btCollisionWorld::AllHitsRayResultCallback rayCallback(rayFrom, rayTo);
	rayCallback.m_collisionFilterGroup = collisionGroupMask;
	rayCallback.m_collisionFilterMask = collisionFilterMask;
	
	// perform the raycast
	m_world->rayTest(rayFrom, rayTo, rayCallback);

	if(m_useDebugDrawer)
		m_debugDrawer->drawLine(rayFrom, rayTo, btVector3(0,1,0));
	
	if (rayCallback.hasHit())
	{
		auto& pObjs = rayCallback.m_collisionObjects;
		auto& pCollPoints = rayCallback.m_hitPointWorld;
	
		for(uint32_t i {0}; i < pObjs.size(); ++i)
		{
			RayResult res;
			auto object = pObjs.at(i);
			res.pObj = const_cast<btCollisionObject*>(object);
			res.hitPoint = pCollPoints.at(i*3);
			res.node = static_cast<Node*>(object->getUserPointer());
			output.push_back(res);
		}

		// set the result data

		return true;
	}

	// we didn't hit anything
	return false;
}

bool 
HyperEngine::checkRaycastCollisionWithNode(const btVector3 &startPosition, const btVector3 &direction)
{
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

	// Callbacks, guardar el puntero a la engine en el user pointer de window
	glfwSetWindowUserPointer( m_window, this );

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
		HyperEngine* engine = static_cast<HyperEngine*>( glfwGetWindowUserPointer( window ) );
		engine->setKeyState(key, action);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
		HyperEngine* engine = static_cast<HyperEngine*>( glfwGetWindowUserPointer( window ) );
		engine->setMouseKeyState(button, action);
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset){
		HyperEngine* engine = static_cast<HyperEngine*>( glfwGetWindowUserPointer( window ) );
		engine->setMouseWheelStatus(xoffset, yoffset);
	});

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
	using DBG = DebugDrawer;
	m_debugDrawer->setDebugMode(
			DBG::DBG_DrawWireframe 
		| 	DBG::DBG_DrawAabb 
		| 	DBG::DBG_DrawContactPoints
	);
	m_world->setDebugDrawer(m_debugDrawer);
	// debug de cuántos contactos hay entre objetos
	m_world->setInternalTickCallback([](btDynamicsWorld *world, btScalar timeStep) {
		auto engine = static_cast<HyperEngine*>(world->getWorldUserInfo());
		auto& collisionPairs = engine->m_collisionPairs;
		collisionPairs.clear();

		// TODO:: ver cómo interpretar los contactos de colisiones

		int numManifolds = world->getDispatcher()->getNumManifolds();
		// INFOLOG( "TickCallback: numManifolds = " VAR(numManifolds) )
		for (int i {0}; i < numManifolds; ++i)
		{
			auto* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
			auto* obA = contactManifold->getBody0();
			auto* obB = contactManifold->getBody1();
			auto* nodeA = static_cast<Node*>(obA->getUserPointer());
			auto* nodeB = static_cast<Node*>(obB->getUserPointer());

			CollisionPairResult pair;
			pair.IDs = std::make_pair(nodeA->getNameID(), nodeB->getNameID());

			// TODO:: recordar que pt.getDistance() < 0.0f nos dice si la colisión es interna y no solo el AABB
			// y así conseguiríamos una detección de colisión más precisa

			int numContacts = contactManifold->getNumContacts();
			// int deepcontacts {0};

			// INFOLOG("numContacts" << VAR(numContacts) )

			for (int j {0}; j < numContacts; ++j)
			{
				auto& pt = contactManifold->getContactPoint(j);
				CollisionPoint point { .pointA{pt.getPositionWorldOnA()}, .pointB{pt.getPositionWorldOnB()}, .normalOnB{pt.m_normalWorldOnB} };
				pair.points.push_back(std::move(point));

				// INFOLOG( "TickCallback: pt.getDistance() = " VAR(pt.getDistance()) )
				
				// auto ptA = pt.getPositionWorldOnA();
				// auto ptB = pt.getPositionWorldOnB();
				// auto normalOnB = pt.m_normalWorldOnB;
				// INFOLOG( "ptA = " << VAR( ptA.getX() ) << VAR( ptA.getY() )<< VAR( ptA.getZ() ) )
				// INFOLOG( "ptB = " << VAR( ptB.getX() ) << VAR( ptB.getY() )<< VAR( ptB.getZ() ) )
				// INFOLOG( "normalOnB = " << VAR( normalOnB.getX() ) << VAR( normalOnB.getY() )<< VAR( normalOnB.getZ() ) )


				// if (pt.getDistance() < 0.0f)
				// {
				// 	++deepcontacts;
				// 	Vector3 ptA = pt.PositionWorldOnA;
				// 	Vector3 ptB = pt.PositionWorldOnB;
				// 	Vector3 normalOnB = pt.NormalWorldOnB;
				// }
			}

			// INFOLOG("deepcontacts" << VAR(deepcontacts) )

			collisionPairs.push_back(std::move(pair));
			contactManifold->clearManifold();
		}


		// INFOLOG( "TickCallback: collisionPairs.size() = " VAR( collisionPairs.size() ) )
	}, this);

	// poner gravedad
	m_world->setGravity( btVector3(0, -9.81f, 0) );
}

void 
HyperEngine::setKeyState(int const key, int const action)
{
	m_keystates[key] = action;
}

void 
HyperEngine::setMouseKeyState(int const key, int const action)
{
	m_mousekeystates[key] = action;
}

void 
HyperEngine::setMouseWheelStatus(float const offsetX, float const offsetY)
{
	m_mouseWheelStatus.offsetX = offsetX;
	m_mouseWheelStatus.offsetY = offsetY;
}

void 
HyperEngine::resetKeyStates(void)
{
	// Para cada tecla reconocida en el mapa, setea a invalid (Estados: -1 invalid, 0 release, 1 press, 2 hold)
	for(auto iter = m_keystates.begin(); iter != m_keystates.end(); ++iter)
		iter->second = -1;
}

void 
HyperEngine::resetMouseKeyStates(void)
{
	for(auto iter = m_mousekeystates.begin(); iter != m_mousekeystates.end(); ++iter)
		iter->second = -1;
}

void
HyperEngine::resetMouseWheelStatus(void)
{
	m_mouseWheelStatus.offsetX = 0.0f;
	m_mouseWheelStatus.offsetY = 0.0f;
}

}
