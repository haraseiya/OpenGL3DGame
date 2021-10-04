#pragma once
#include "GameObject.h"

class InstanceMeshComponent;

class Bullet : public GameObject
{
public:
	Bullet(const Vector3& pos, const Vector3& dir,Tag tag,float speed,float scale);
	~Bullet();

	void UpdateActor(float deltaTime);
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox);

private:
	InstanceMeshComponent* mInstanceMeshComp;

	float mLifeTime;
	Tag mTag;
};

