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
	// NPCが存在していれば走りアニメーションに移行
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
