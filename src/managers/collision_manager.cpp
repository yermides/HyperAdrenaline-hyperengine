#include "collision_manager.hpp"

CollisionManager::CollisionManager(/* args */)
{
	initialize();
}

CollisionManager::~CollisionManager()
{
    //delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_solver;

	//delete broadphase
	delete m_overlappingPairCache;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;

	for(uint32_t i {0}; i < m_collisionShapes.size(); ++i)
		delete m_collisionShapes[i];

	m_collisionShapes.clear();
}

// Private functions

void 
CollisionManager::initialize(void)
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	// Poner gravedad y tal
	m_dynamicsWorld->setGravity({0.0f, -9.81f, 0.0f});
	m_dynamicsWorld->stepSimulation(1.0f / 60.0f);

}