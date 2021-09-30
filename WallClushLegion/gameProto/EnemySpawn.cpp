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
		return EnemyStateEnum::Idle;
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

