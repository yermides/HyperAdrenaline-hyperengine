#pragma once
#include <bullet/btBulletCollisionCommon.h>

// Clase que hereda de la transformación del mundo de bullet para setear la info de bullet a los gráficos y viceversa
struct OpenGLMotionState : public btDefaultMotionState {
	OpenGLMotionState(const btTransform &transform) 
    : btDefaultMotionState(transform) 
    {
    }

	void GetWorldTransform(btScalar* transform) 
    {
		btTransform trans;
		getWorldTransform(trans);
		trans.getOpenGLMatrix(transform);
	}
};

// Estructura que almacena todos los datos físicos, contenida en cada nodo
struct PhysicProperties {
    btRigidBody*        m_body              { nullptr };
    btCollisionShape*   m_collisionShape    { nullptr };
    OpenGLMotionState*  m_motionState       { nullptr };

    PhysicProperties()
    :   m_body(nullptr), m_collisionShape(nullptr), m_motionState(nullptr)
    {
    }

    PhysicProperties(btRigidBody* body, btCollisionShape* collisionShape, OpenGLMotionState* motionState)
    :   m_body(body), m_collisionShape(collisionShape), m_motionState(motionState)
    {
    }
};

struct MyBulletContactResult : public btCollisionWorld::ContactResultCallback
{
    virtual btScalar addSingleResult(
            btManifoldPoint& cp 
        ,   const btCollisionObject* colObj0
        ,   int partId0
        ,   int index0
        ,   const btCollisionObject* colObj1
        ,   int partId1
        ,   int index1
    ) final
    {
        btVector3 ptA = cp.getPositionWorldOnA();
        btVector3 ptB = cp.getPositionWorldOnB();
        return 0;
    }
};
