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
	// �A�j���[�V�������I�������瑖�胂�[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Run;
	}

	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	// ����A�j���Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1);
	//mOwnerActor->SetAttackHitBox();
}

void EnemyAttack::OnExit()
{
	mOwnerActor->RemoveAttackHitBox();
}
