#include "EnemyIdle.h"

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
	// 体力が0以下なら死亡状態
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// アニメーションが終了したらランダムで
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
}

void EnemyIdle::OnExit()
{
}
