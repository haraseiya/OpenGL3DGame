#include "EnemyDeath.h"
#include "ExplosionEffect.h"

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
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Invalid;
	}

	return EnemyStateEnum::Death;
}

void EnemyDeath::OnEnter()
{
	// ���S�A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Death);
	//mOwnerActor->RemoveHitBox();
}

void EnemyDeath::OnExit()
{
	// �A�j���[�V�����I�����A�I�u�W�F�N�g�j��
	mOwnerActor->SetState(GameObject::STATE_DEAD);
	mExplosion = new ExplosionEffect(mOwnerActor->GetPosition());
}
