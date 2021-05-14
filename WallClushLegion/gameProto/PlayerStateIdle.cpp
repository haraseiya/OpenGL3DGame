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
	//if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	//{
	//	return PlayerState::PLAYER_STATE_ATTACK;
	//}

	// �R���g���[�����͂��ꂽ��
	bool isControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	//�����L�[�����͂��ꂽ��
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isControllerInputOff;

	// �A�C�h����Ԃł͂Ȃ��ꍇ
	if (!IsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateIdle::Enter(class Player* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
