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
	// �U�����[�V�������I�������Ǐ]���[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying()) { return NPCStateEnum::Run; }

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// �ҋ@�A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle, 0.5f);
}

void NPCIdle::OnExit()
{
}
