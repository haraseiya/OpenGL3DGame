#include "PlayerStateAttack.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"

PlayerStateAttack::PlayerStateAttack()
{
}

PlayerStateAttack::~PlayerStateAttack()
{
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerState PlayerStateAttack::Update(Player* owner, float deltaTime)
{
	// B�{�^���������ꂽ��
	const bool finishAnim = !owner->GetSkeletalMeshComp()->IsPlaying();
	if (finishAnim)
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}
	return PlayerState::PLAYER_STATE_ATTACK;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateAttack::Enter(class Player* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK));
}

