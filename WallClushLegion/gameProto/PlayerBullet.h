#pragma once

#include "GameObject.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;
class PlayerBase;
class ObjectPool;

class PlayerBullet :public GameObject
{
public:
	PlayerBullet(PlayerBase* player);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;

private:
	InstanceMeshComponent* mInstanceMeshComp;
	float mLifeTime;
};

