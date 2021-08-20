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
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

	// 方向キーが入力されたか
	mIsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		mIsControllerInputOff;

	// カメラからみた前進方向を取得
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // 高さ方向を無視

	// カメラ前方ベクトルと右方向ベクトル算出
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	float forwardAngle = 0.0f;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));
	Vector2 stickR = INPUT_INSTANCE.GetRStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickR = Vector2::Transform(stickR, rot);

	Vector3 DirVecR(0.0f, 0.0f, 0.0f);
	DirVecR.x += stickR.x;
	DirVecR.y += stickR.y;

	Vector3 charaForwardVec = owner->GetForward(); // キャラの前進ベクトル

	if (DirVecR.LengthSq() > 0.5f)
	{
		// 方向キー入力
		charaForwardVec = DirVecR;

		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);
	}

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
