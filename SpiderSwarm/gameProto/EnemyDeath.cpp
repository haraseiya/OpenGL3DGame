#include "EnemyDeath.h"
#include "ExplosionEffect.h"
#include "Score.h"

EnemyDeath::EnemyDeath(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
	, mExplosion(nullptr)
{
	mStateType = EnemyStateEnum::Death;
}

EnemyDeath::~EnemyDeath()
{
	delete mExplosion;
}

EnemyStateEnum EnemyDeath::Update(float deltaTime)
{
	// ���S�A�j���[�V�������I��������
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Invalid;
	}

	return EnemyStateEnum::Death;
}

void EnemyDeath::OnEnter()
{
	// ���S�A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Death);
	//mOwner->RemoveCollider();
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK) 
	//{ 
	//	mScore = 1000; 
	//}
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG) 
	//{	
	//	mScore = 5000;
	//}
	//if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	//{
	//	mScore = 20000;
	//}
	//mSumScore += mScore;
}

void EnemyDeath::OnExit()
{
	// �A�j���[�V�����I�����A�I�u�W�F�N�g�j��
	mOwner->SetState(GameObject::STATE_DEAD);
	mExplosion = new ExplosionEffect(mOwner->GetPosition());
}
