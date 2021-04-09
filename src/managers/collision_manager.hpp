/*
    DISCLAIMER
    Esto es muy temporal y no funciona de momento, de hecho, no debería de ser un collisionmanager
    sino que debería de haber un collisionbox en los nodos y las colisiones comprobarse gracias al hyperengine.cpp
*/

#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

struct CollisionManager
{
    CollisionManager(/* args */);
    ~CollisionManager();

    // De momento públicas solo para debug
    btDefaultCollisionConfiguration*        m_collisionConfiguration    { nullptr };
    btCollisionDispatcher*                  m_dispatcher                { nullptr };
    btBroadphaseInterface*                  m_overlappingPairCache      { nullptr };
    btSequentialImpulseConstraintSolver*    m_solver                    { nullptr };
    btDiscreteDynamicsWorld*                m_dynamicsWorld             { nullptr };

	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

private:
    void initialize(void);
};
