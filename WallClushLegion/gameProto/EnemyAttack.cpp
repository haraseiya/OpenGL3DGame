#include "EnemyAttack.h"
#include "NPCActorBase.h"
#include "BossEnemy.h"

EnemyAttack::EnemyAttack(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
	, mAnimationSpeed(1.0f)
{
	mStateType = EnemyStateEnum::Attack1;
}

EnemyAttack::~EnemyAttack()
{
}

EnemyStateEnum EnemyAttack::Update(float deltaTime)
{
	// アニメーションが終了したら走りモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		//mOwnerActor->RemoveAttackHitBox();
		return EnemyStateEnum::Idle;
	}
	
	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	printf("攻撃アニメーション開始\n");

	// 攻撃アニメーション再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1, mAnimationSpeed);

	// アタックボックスのセット
	mOwnerActor->SetAttackHitBox();
}

void EnemyAttack::OnExit()
{
	mOwnerActor->RemoveAttackHitBox();
	printf("攻撃アニメーション終了\n");
}
