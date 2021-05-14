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

// アイドル状態から他の状態への移行
PlayerState PlayerStateAttack::Update(Player* owner, float deltaTime)
{
	// Bボタンが押されたら
	const bool finishAnim = !owner->GetSkeletalMeshComp()->IsPlaying();
	if (finishAnim)
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}
	return PlayerState::PLAYER_STATE_ATTACK;
}

// アイドル状態への移行処理
void PlayerStateAttack::Enter(class Player* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_ATTACK));
}

