#include "NPCIdle.h"
#include "Player.h"

NPCIdle::NPCIdle(NPCBehaviorComponent* owner,Player* player)
	: NPCState(owner)
	, mPlayer(player)
{
}

NPCIdle::~NPCIdle()
{
}

NPCStateEnum NPCIdle::Update(float deltaTime)
{
	// �ҋ@�A�j���[�V�������I����
	//if (!mOwnerActor->IsAnimationPlaying()) { return NPCStateEnum::Run; }
	Vector3 distance = mOwnerActor->GetPosition() - mPlayer->GetPosition();
	abs(distance.x);
	abs(distance.y);

	if (distance.x > 100 && distance.y > 100)
	{
		return NPCStateEnum::Run;
	}

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
