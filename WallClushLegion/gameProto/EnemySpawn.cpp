#include "EnemySpawn.h"

EnemySpawn::EnemySpawn(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Spawn;
}

EnemySpawn::~EnemySpawn()
{
}

EnemyStateEnum EnemySpawn::Update(float deltaTime)
{
	// �A�j���[�V�����I�����A����A�j���[�V�����Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		// �{�X�G�ł���Ι��K�A����ȊO�Ȃ�ҋ@��Ԃɔ�΂�
		if (mOwnerActor->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
		{
			return EnemyStateEnum::Roar;
		}
		else
		{
			return EnemyStateEnum::Idle;
		}
	}

	return EnemyStateEnum::Spawn;
}

void EnemySpawn::OnEnter()
{
	// �o���A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Spawn);
}

void EnemySpawn::OnExit()
{

}

