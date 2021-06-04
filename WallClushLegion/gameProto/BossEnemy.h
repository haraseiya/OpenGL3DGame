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
	BossEnemy(NPCActorBase* npc);
	~BossEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void RemoveAttackHitBox()override;

	void HitNPC(BoxCollider* enemyBox,BoxCollider* npcBox);

	bool IsFrontHit();

private:
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* m_enemyBehaviorComponent;

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mTime;
};