#pragma once
#include "GameObject.h"

class EnemyBase;
class InstanceMeshComponent;

class EnemyBullet : public GameObject
{
public:
	EnemyBullet(EnemyBase* enemy, const Vector3& direction, float scale, float speed);
	~EnemyBullet();

	void UpdateActor(float deltaTime);
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox);

private:
	InstanceMeshComponent* mInstanceMeshComp;

	Tag mTag;
	float mLifeTime;
	static const float mMaxLifeTime;
};

