#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Object
{
public:
	Object();
	Object(glm::vec3 pos, glm::vec3 rot); // Position of object/ rotation.
	~Object();  // Destructor Called when Ever Out of Scope/ Bounds  Or manual to Free Resource.

	glm::vec3 position;
	glm::vec3 rotation;

	// Colliders When ever an element Collides with another element.
	//SphereCollider* collider;
	//SphereCollider* GetCollider();
	//void AttachCollider(SphereCollider* attachingCollider);

	virtual void Setup() = 0;
	virtual void Draw() = 0;
	virtual void Update(float);
};