#include "Object.h"

Object::Object()
{
}

Object::Object(glm::vec3 pos)
{
	position = pos;
}

Object::~Object()
{
}

/*SphereCollider* GameObject::GetCollider()
{
	return collider;
}

void GameObject::AttachCollider(SphereCollider* attachingCollider)
{
	collider = attachingCollider;
}*/

void Object::Update(float deltaTime)
{
}