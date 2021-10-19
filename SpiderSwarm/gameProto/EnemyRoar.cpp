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
	const bool isDead = mOwnerActor->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// プレイヤーが存在していれば走りアニメーションに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	return EnemyStateEnum::Roar;
}

void EnemyRoar::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Roar);
}

void EnemyRoar::OnExit()
{
}
