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

// El objeto puede ser un rigidbody (complejo) o tan solo un collisionObject, lo de ghost object es una especificación del CO
union PhysicsData {
    btRigidBody*        body;
    btCollisionObject*  collObj;
};

// Estructura que almacena todos los datos físicos, contenida en cada nodo
struct PhysicProperties {
    enum PhysicDatatype { COLLISION_OBJECT, RIGID_BODY };

    PhysicDatatype      m_type;
    PhysicsData         m_data;

    // btRigidBody*        m_body              { nullptr };
    btCollisionShape*   m_collisionShape    { nullptr };
    OpenGLMotionState*  m_motionState       { nullptr };
};

// using CollisionPoint = btManifoldPoint;
struct CollisionPoint {
    btVector3 pointA, pointB, normalOnB;
};
struct CollisionPairResult {
    std::pair<int, int> IDs;
    std::vector<CollisionPoint> points;
};

// struct MyBulletContactResult : public btCollisionWorld::ContactResultCallback
// {
//     virtual btScalar addSingleResult(
//             btManifoldPoint& cp 
//         ,   const btCollisionObject* colObj0
//         ,   int partId0
//         ,   int index0
//         ,   const btCollisionObject* colObj1
//         ,   int partId1
//         ,   int index1
//     ) final
//     {
//         btVector3 ptA = cp.getPositionWorldOnA();
//         btVector3 ptB = cp.getPositionWorldOnB();
//         return 0;
//     }
// };
