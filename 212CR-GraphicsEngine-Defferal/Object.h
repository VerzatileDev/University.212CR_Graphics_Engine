#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Object
{
public:
	Object();
	Object(glm::vec3 pos);
	~Object();

	glm::vec3 position;

	//SphereCollider* collider;
	//SphereCollider* GetCollider();
	//void AttachCollider(SphereCollider* attachingCollider);

	virtual void Setup() = 0;
	virtual void Draw() = 0;
	virtual void Update(float);
};