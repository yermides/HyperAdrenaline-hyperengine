#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "entities/entity.hpp"

struct Node
{
	using NodeList = std::vector<Node*>;
	
	Node(/* args */);
	~Node();

	void                addChild(Node* node);
	void                removeChild(Node* node);
	void                setMatrixTransform(const glm::mat4& newTransform);
	const glm::mat4&    getMatrixTransform(void);

	void                setEntity(Entity* newEntity);
	Entity*             getEntity(void);
	void                setParent(Node* newNode);
	Node*               getParent(void);

	void                setTranslation(const glm::vec3& newTranslation);
	const glm::vec3&    getTranslation(void);
	void                setRotation(const glm::vec3& newRotation);
	const glm::vec3&    getRotation(void);
	void                setScale(const glm::vec3& newScale);
	const glm::vec3&    getScale(void);

	void                translate(const glm::vec3& accumulation);
	void                rotate(const glm::vec3& accumulation);
	void                scale(const glm::vec3& accumulation);

	void                traverse(const glm::mat4& accumulatedTrans);

private:
	glm::mat4 m_transform;
	Entity* m_entity;
	NodeList m_childs;
	Node* m_parent;

	glm::vec3 m_translation;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	bool m_wantsUpdate { true };
};
