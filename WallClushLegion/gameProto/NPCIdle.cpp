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
	// 待機アニメーションが終われば
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
	// 待機アニメーション再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle, 0.5f);
}

void NPCIdle::OnExit()
{
}
