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
	// スティック入力が入ったか
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove())&&!(INPUT_INSTANCE.IsRStickMove());

	// 方向キーが入力されたか
	mIsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		mIsControllerInputOff;

	// アイドル状態ではない場合
	if (!mIsIdle)
	{
		return PlayerState::PLAYER_STATE_RUN;
	}

	return PlayerState::PLAYER_STATE_IDLE;
}

// アイドル状態への移行処理
void PlayerStateIdle::Enter(class Player* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	mMeshComp = owner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}
