#include "EnemyIdle.h"

EnemyIdle::EnemyIdle(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
{
	mStateType = EnemyStateEnum::Idle;
}

EnemyIdle::~EnemyIdle()
{
}

EnemyStateEnum EnemyIdle::Update(float deltaTime)
{
	// �̗͂�0�ȉ��Ȃ玀�S���
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Run;
	}

	return EnemyStateEnum::Idle;
}

void EnemyIdle::OnEnter()
{
	mOwner->PlayAnimation(EnemyStateEnum::Idle);
}

void EnemyIdle::OnExit()
{
}
