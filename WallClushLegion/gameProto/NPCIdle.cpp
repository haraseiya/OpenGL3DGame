#include "NPCIdle.h"

NPCIdle::NPCIdle(NPCBehaviorComponent* owner)
	: NPCState(owner)
{
}

NPCIdle::~NPCIdle()
{
}

NPCStateEnum NPCIdle::Update(float deltaTime)
{
	// 攻撃モーションが終わったら追従モードに移行
	if (!mOwnerActor->IsAnimationPlaying()) { return NPCStateEnum::Run; }

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// 待機アニメーション再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle, 0.5f);
}

void NPCIdle::OnExit()
{
}
