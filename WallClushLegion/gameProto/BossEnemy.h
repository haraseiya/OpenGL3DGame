#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;

class BossEnemy : public EnemyBase
{
public:
	BossEnemy(Player* player);
	~BossEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void RemoveAttackHitBox()override;

	void HitNPC(BoxCollider* enemyBox,BoxCollider* npcBox);

	bool IsFrontHit();

private:
	BoxCollider* m_attackBox;
	EnemyBehaviorComponent* m_enemyBehaviorComponent;

	AABB m_enemyBox;
};