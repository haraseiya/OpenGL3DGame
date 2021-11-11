#include "EnemyIdle.h"
#include <iostream>
#include <mutex>

EnemyIdle::EnemyIdle(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
{
	mStateType = EnemyStateEnum::Idle;
	mNextAnimType = Math::GetRandom(1, 2);
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


	// �A�j���[�V�������I�������烉���_���ŃA�j���[�V�����J��
	if (!mOwner->IsAnimationPlaying())
	{
		if (mNextAnimType == 1)
		{
			return EnemyStateEnum::Run;
		}
		if (mNextAnimType == 2)
		{
			return EnemyStateEnum::Attack2;
		}
	}

	return EnemyStateEnum::Idle;
}

void EnemyIdle::OnEnter()
{
	mOwner->PlayAnimation(EnemyStateEnum::Idle);
	mNextAnimType = Math::GetRandom(1, 2);
}

void EnemyIdle::OnExit()
{
	//mOwner->SetCollider();
}
