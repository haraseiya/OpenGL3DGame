#include "EnemyAttack.h"
#include "NPCActorBase.h"
#include "BossEnemy.h"

EnemyAttack::EnemyAttack(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack1;
}

EnemyAttack::~EnemyAttack()
{
}

EnemyStateEnum EnemyAttack::Update(float deltaTime)
{
	// 攻撃モーションが終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		mOwnerActor->RemoveAttackHitBox();
		return EnemyStateEnum::Run;
	}

	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	// 走りアニメ再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1);
	//mOwnerActor->SetAttackHitBox();
}

void EnemyAttack::OnExit()
{
	mOwnerActor->RemoveAttackHitBox();
}
