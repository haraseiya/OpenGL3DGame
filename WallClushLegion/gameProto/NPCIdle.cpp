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
	// �v���C���[�ւ̌��������߂�
	mNPCPos = mOwnerActor->GetPosition();
	mPlayerPos = mPlayer->GetPosition();
	mDistance = mNPCPos - mPlayerPos;
	abs(mDistance.x);
	abs(mDistance.y);

	// �v���C���[�Ƃ̋�����������x����Ă����
	if (mDistance.x > 50.0f || mDistance.y > 50.0f)
	{
		return NPCStateEnum::Run;
	}

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// �ҋ@�A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle);
}

void NPCIdle::OnExit()
{
}
