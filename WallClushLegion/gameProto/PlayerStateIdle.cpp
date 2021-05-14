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

// アイドル状態から他の状態への移行
PlayerState PlayerStateIdle::Update(Player* owner, float deltaTime)
{
	//if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	//{
	//	return PlayerState::PLAYER_STATE_ATTACK;
	//}

	// コントローラ入力されたか
	bool isControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	//方向キーが入力されたか
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isControllerInputOff;

	// アイドル状態ではない場合
	if (!IsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// アイドル状態への移行処理
void PlayerStateIdle::Enter(class Player* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
