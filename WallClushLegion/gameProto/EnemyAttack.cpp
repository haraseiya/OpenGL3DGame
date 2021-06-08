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
	// �A�j���[�V�������I�������瑖�胂�[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		//mOwnerActor->RemoveAttackHitBox();
		return EnemyStateEnum::Idle;
	}
	
	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	printf("�U���A�j���[�V�����J�n\n");

	// �U���A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1, mAnimationSpeed);

	// �A�^�b�N�{�b�N�X�̃Z�b�g
	mOwnerActor->SetAttackHitBox();
}

void EnemyAttack::OnExit()
{
	mOwnerActor->RemoveAttackHitBox();
	printf("�U���A�j���[�V�����I��\n");
}
