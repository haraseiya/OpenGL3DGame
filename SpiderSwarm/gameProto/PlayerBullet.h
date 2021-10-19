#pragma once

#include "BulletBase.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;
class PlayerBase;
class ObjectPool;

class PlayerBullet :public BulletBase
{
public:
	PlayerBullet(PlayerBase* player, const Vector3& pos, const Vector3& dir);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;

	void SetCollider();

private:
	float mLifeTime;
};

