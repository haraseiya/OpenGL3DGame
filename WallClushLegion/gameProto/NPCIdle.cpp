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

	// ������x����Ă����Run��ԂɈڍs
	const bool isFar = Math::Abs(mDistance.x) > 150.0f && Math::Abs(mDistance.y) > 150.0f;
	if (isFar)
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
