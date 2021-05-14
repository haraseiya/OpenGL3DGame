#include "EnemyIdle.h"

EnemyIdle::EnemyIdle(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Idle;
}

EnemyIdle::~EnemyIdle()
{
}

EnemyStateEnum EnemyIdle::Update(float deltaTime)
{
	// 攻撃モーションが終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}

	return EnemyStateEnum::Idle;
}

void EnemyIdle::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Idle);
}

void EnemyIdle::OnExit()
{
}
