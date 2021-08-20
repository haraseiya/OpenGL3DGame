#include "PlayerStateIdle.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"

PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerState PlayerStateIdle::Update(Player* owner, float deltaTime)
{
	// �X�e�B�b�N���͂���������
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove())&&!(INPUT_INSTANCE.IsRStickMove());

	// �����L�[�����͂��ꂽ��
	mIsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		mIsControllerInputOff;

	// �A�C�h����Ԃł͂Ȃ��ꍇ
	if (!mIsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateIdle::Enter(class Player* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	mMeshComp = owner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
