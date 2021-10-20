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
	// �̗͂�0�ȉ��Ȃ玀�S���
	const bool isDead = mOwnerActor->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// �A�j���[�V�������I��������
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
