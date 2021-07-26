#pragma once
#include "EnemyBase.h"
#include "BoxCollider.h"

class Player;
class NPCActorBase;
class EnemyBehaviorComponent;
class BoxCollider;

class WeakEnemy : public EnemyBase
{
public:
	WeakEnemy(GameObject* target);
	~WeakEnemy();

	void UpdateActor(float deltaTime) override;
	void OnCollisionEnter(ColliderComponent* other) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) override;

	void SetAttackHitBox(float scale) override;
	void RemoveAttackHitBox()override;

	bool IsFrontHit();

private:
	void LoadModel();			// ���f���ǂݍ���
	void LoadSkeleton();		// �X�P���g���ǂݍ���
	void LoadAnimation();		// �A�j���[�V�����ǂݍ���
	void BehaviorResister();	// �U�镑���̒ǉ�
	void SetCollider();			// ���g�̓����蔻���ǉ�
	void SetAttackTrigger();	// �A�^�b�N�g���K�[��ǉ�

	BoxCollider* mBoxCollider;
	BoxCollider* mAttackTrigger;
	BoxCollider* mAttackBox;
	EnemyBehaviorComponent* m_enemyBehaviorComponent;
	GameObject* mTarget;

	AABB mEnemyBox;
	AABB mEnemyForward;
	AABB mEnemyAttackTrigger;

	float mCoolTime;
};