#include "NPCDie.h"

NPCDie::NPCDie(NPCBehaviorComponent* owner)
	: NPCState(owner)
{
	mStateType = NPCStateEnum::Die;
}

NPCDie::~NPCDie()
{
}

NPCStateEnum NPCDie::Update(float deltaTime)
{
	// 詠唱が行われたら走りアニメーションに移行
	

	return NPCStateEnum::Die;
}

void NPCDie::OnEnter()
{
	// 走りアニメ再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Die, 0.5f);
}

void NPCDie::OnExit()
{
}
