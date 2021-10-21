#include "EnemyRoar.h"

EnemyRoar::EnemyRoar(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Roar;
}

EnemyRoar::~EnemyRoar()
{
}

EnemyStateEnum EnemyRoar::Update(float deltaTime)
{
	// 体力が0以下なら死亡状態
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// アニメーションが終了したら
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	return EnemyStateEnum::Roar;
}

void EnemyRoar::OnEnter()
{
	mOwner->PlayAnimation(EnemyStateEnum::Roar);
}

void EnemyRoar::OnExit()
{
}
