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
	// �r�����s��ꂽ�瑖��A�j���[�V�����Ɉڍs
	

	return NPCStateEnum::Die;
}

void NPCDie::OnEnter()
{
	// ����A�j���Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Die, 0.5f);
}

void NPCDie::OnExit()
{
}
