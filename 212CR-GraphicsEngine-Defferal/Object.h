#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Object
{
public:
	Object();
	Object(glm::vec3 pos, glm::vec3 rot); // Position of object as well as rotation.
	~Object();

	glm::vec3 position;
	glm::vec3 rotation;

	//SphereCollider* collider;
	//SphereCollider* GetCollider();
	//void AttachCollider(SphereCollider* attachingCollider);

	virtual void Setup() = 0;
	virtual void Draw() = 0;
	virtual void Update(float);
};