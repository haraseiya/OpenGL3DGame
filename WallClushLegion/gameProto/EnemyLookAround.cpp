#include "EnemyLookAround.h"

EnemyLookAround::EnemyLookAround(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::LookAround;
}

EnemyLookAround::~EnemyLookAround()
{
}

EnemyStateEnum EnemyLookAround::Update( float deltaTime)
{
	// アニメーション再生終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// 続行
	return EnemyStateEnum::LookAround;
}

void EnemyLookAround::OnEnter()
{
	// 見渡しアニメ再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::LookAround);
}

void EnemyLookAround::OnExit()
{
}