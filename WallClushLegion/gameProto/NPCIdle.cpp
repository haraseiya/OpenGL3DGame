#include "NPCIdle.h"
#include "Player.h"

NPCIdle::NPCIdle(NPCBehaviorComponent* owner,Player* player,EnemyBase* enemy)
	: NPCState(owner)
	, mPlayer(player)
	, mEnemy(enemy)
{
	mStateType = NPCStateEnum::Idle;
}

NPCIdle::~NPCIdle()
{
}

NPCStateEnum NPCIdle::Update(float deltaTime)
{
	// プレイヤーへの向きを求める
	mNPCPos = mOwnerActor->GetPosition();
	mPlayerPos = mPlayer->GetPosition();
	mDistance = mNPCPos - mPlayerPos;

	// ある程度離れていればRun状態に移行
	const bool isFar = Math::Abs(mDistance.x) > 150.0f && Math::Abs(mDistance.y) > 150.0f;
	if (isFar)
	{
		return NPCStateEnum::Run;
	}

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// 待機アニメーション再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle);
}

void NPCIdle::OnExit()
{
}
