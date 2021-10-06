#pragma once
#include "GameObject.h"

class EnemyBase;
class InstanceMeshComponent;

class EnemyBullet : public GameObject
{
public:
	EnemyBullet(EnemyBase* enemy);
	~EnemyBullet();

	void UpdateActor(float deltaTime);
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox);

private:
	InstanceMeshComponent* mInstanceMeshComp;

	Tag mTag;
	float mLifeTime;
	bool mIsUse;	// Žg—p‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
};

