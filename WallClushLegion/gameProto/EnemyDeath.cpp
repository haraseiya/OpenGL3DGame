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
	return EnemyStateEnum::Death;
}

void EnemyDeath::OnEnter()
{
	// 死亡アニメーション再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Death);
}

void EnemyDeath::OnExit()
{
	// アニメーション終了時、オブジェクト破棄
	mOwnerActor->SetState(GameObject::STATE_DEAD);
	mExplosion = new ExplosionEffect(mOwnerActor->GetPosition());
}
