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
	abs(mDistance.x);
	abs(mDistance.y);

	// プレイヤーとの距離がある程度離れていれば
	if (mDistance.x > 50.0f || mDistance.y > 50.0f)
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
