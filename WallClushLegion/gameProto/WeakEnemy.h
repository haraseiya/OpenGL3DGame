#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(GameObject* target);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void RemoveAttackHitBox()override;

	void RemoveHitBox() override;
	bool IsFrontHit();

private:
	void LoadModel();			// モデル読み込み
	void LoadSkeleton();		// スケルトン読み込み
	void LoadAnimation();		// アニメーション読み込み
	void BehaviorResister();	// 振る舞いの追加
	void SetCollider();			// 自身の当たり判定を追加
	void SetAttackTrigger();	// アタックトリガーを追加

	BoxCollider* mBoxCollider;
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;

	ExplosionEffect* mExplosion;	// 爆発エフェクト

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mCoolTime;

	static const float mInterval;
	float mTimer;
};