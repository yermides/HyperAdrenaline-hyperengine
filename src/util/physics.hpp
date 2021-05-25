#pragma once
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

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
    btPairCachingGhostObject * ghostObj;
};

// Estructura que almacena todos los datos físicos, contenida en cada nodo
struct PhysicProperties {
    enum PhysicDatatype { COLLISION_OBJECT, RIGID_BODY, KINEMATIC_CHARACTER };

    PhysicDatatype      m_type;
    PhysicsData         m_data;

    // btRigidBody*        m_body              { nullptr };
    btCollisionShape*   m_collisionShape    { nullptr };
    OpenGLMotionState*  m_motionState       { nullptr };
    btKinematicCharacterController* charCon { nullptr };

    ~PhysicProperties()
    {
        switch (m_type)
        {
        case COLLISION_OBJECT:
            /* code */
            if(m_data.collObj) delete m_data.collObj;
            m_data.collObj = nullptr;
            break;

        case RIGID_BODY:
            if(m_data.body) delete m_data.body;
            m_data.body = nullptr;
            /* code */
            break;

        case KINEMATIC_CHARACTER:
            if(m_data.ghostObj) delete m_data.ghostObj;
            m_data.ghostObj = nullptr;
            /* code */
            break;
        
        default:
            break;
        }

        if(charCon) delete charCon;
            charCon = nullptr;
        if(m_collisionShape) delete m_collisionShape;
            m_collisionShape = nullptr;
        // if(m_motionState) delete m_motionState;
            m_motionState = nullptr;
    };
    
};

// using CollisionPoint = btManifoldPoint;
struct CollisionPoint {
    btVector3 pointA, pointB, normalOnB;
    float distance;
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
