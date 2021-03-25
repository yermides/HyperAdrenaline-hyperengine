#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

struct CollisionManager
{
    CollisionManager(/* args */);
    ~CollisionManager();
private:
    void initialize(void);

    btDefaultCollisionConfiguration*        m_collisionConfiguration    { nullptr };
    btCollisionDispatcher*                  m_dispatcher                { nullptr };
    btBroadphaseInterface*                  m_overlappingPairCache      { nullptr };
    btSequentialImpulseConstraintSolver*    m_solver                    { nullptr };
    btDiscreteDynamicsWorld*                m_dynamicsWorld             { nullptr };
};
