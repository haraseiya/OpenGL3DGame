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

	bool IsFrontHit();

private:
	void LoadModel();			// モデル読み込み
	void LoadSkeleton();		// スケルトン読み込み
	void LoadAnimation();		// アニメーション読み込み
	void BehaviorResister();	// 振る舞いの追加
	void SetCollider();			// 自身の当たり判定を追加
	void SetAttackTrigger();	// アタックトリガーを追加

	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* m_enemyBehaviorComponent;
	NPCActorBase* mNPC;

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mCoolTime;
};