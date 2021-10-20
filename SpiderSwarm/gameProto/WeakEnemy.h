#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;
class ExplosionEffect;
class EnemyBullet;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(GameObject* target);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* own,ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;

private:
	void LoadModel();			// ���f���ǂݍ���
	void LoadSkeleton();		// �X�P���g���ǂݍ���
	void LoadAnimation();		// �A�j���[�V�����ǂݍ���
	void BehaviorResister();	// �U�镑���̒ǉ�
	void SetCollider();			// ���g�̓����蔻���ǉ�

	BoxCollider* mBoxCollider;
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* mEnemyBehaviorComponent;
	GameObject* mTarget;

	ExplosionEffect* mExplosion;	// �����G�t�F�N�g

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	EnemyBullet* mBullet;

	Vector3 mForwardVec; // �O�i�����x�N�g��  

	float mShootTimer;
	static const float mInterval;
	float mTimer;
};