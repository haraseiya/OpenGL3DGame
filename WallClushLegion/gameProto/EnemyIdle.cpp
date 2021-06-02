#include "EnemyIdle.h"

EnemyIdle::EnemyIdle(EnemyBehaviorComponent* owner,NPCActorBase* npc)
	: EnemyState(owner)
	, mNPC(npc)
{
	mStateType = EnemyStateEnum::Idle;
}

EnemyIdle::~EnemyIdle()
{
}

EnemyStateEnum EnemyIdle::Update(float deltaTime)
{
	// NPC�����݂��Ă���Α���A�j���[�V�����Ɉڍs
	if (mNPC)
	{
		return EnemyStateEnum::Run;
	}

	return EnemyStateEnum::Idle;
}

void EnemyIdle::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Idle);
}

void EnemyIdle::OnExit()
{
}
