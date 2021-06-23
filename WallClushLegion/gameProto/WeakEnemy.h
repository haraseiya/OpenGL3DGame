#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class BoxCollider;
class Player;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(Player* player);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	bool IsFrontHit();

private:
	BoxCollider* mHitBox;
	BoxCollider* mFrontTriggerBox;
};