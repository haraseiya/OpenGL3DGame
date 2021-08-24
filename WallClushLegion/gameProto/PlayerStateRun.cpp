#include "PlayerStateRun.h"
#include "PlayerBase.h"
#include "Input.h"
#include "SkeletalMeshComponent.h"

PlayerStateRun::PlayerStateRun()
{

}

PlayerStateRun::~PlayerStateRun()
{
}

PlayerState PlayerStateRun::Update(PlayerBase* owner, float deltaTime)
{
	// コントローラ入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	//方向キーが入力されたか
	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		isContollerInputOff;

	// 待機状態の場合
	if (IsIdle)
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}

	// 移動処理
	MoveCalc(owner, deltaTime);

	return PlayerState::PLAYER_STATE_RUN;
}

// RUN状態への切り替え処理
void PlayerStateRun::Enter(PlayerBase* owner, float deltaTime)
{
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_RUN));
}

// 移動処理
void PlayerStateRun::MoveCalc(PlayerBase* owner, float deltaTime)
{
	//キャラ入力
	const float speed = 350.0f;
	float charaSpeed = owner->GetSpeed(); //  キャラの現在のスピード

	// カメラからみた前進方向を取得
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // 高さ方向を無視

	// カメラ前方ベクトルと右方向ベクトル算出
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向ベクトルからカメラ回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;

	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	Vector3 charaForwardVec = owner->GetForward(); // キャラの前進ベクトル

	// キャラクター移動の入力キー取得
	Vector3 DirVecL(0.0f, 0.0f, 0.0f);
	Vector3 DirVecR(0.0f, 0.0f, 0.0f);
	//if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	//{
	//	DirVec += forwardVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	//{
	//	DirVec -= forwardVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	//{
	//	DirVec += rightVec;
	//}

	//if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	//{
	//	DirVec -= rightVec;
	//}

	// スティック情報読み込み
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Vector2 stickR = INPUT_INSTANCE.GetRStick();

	Matrix3 rotL = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rotL);
	DirVecL.x += stickL.x;
	DirVecL.y += stickL.y;

	Matrix3 rotR = Matrix3::CreateRotation(forwardAngle);
	stickR = Vector2::Transform(stickR, rotR);
	DirVecR.x += stickR.x/mRoot;
	DirVecR.y += stickR.y/mRoot;

	// 入力キーの総和
	if (DirVecL.LengthSq() > 0.5f)
	{
		// 移動処理
		Vector3 position = owner->GetPosition();

		// 現在のスピードを保存
		charaSpeed = speed * deltaTime;
		position += DirVecL * charaSpeed;

		// キャラの位置・スピード・変換行列の再計算の必要をセット
		owner->SetPosition(position);
		owner->SetSpeed(charaSpeed);
	}

	if (DirVecR.LengthSq() > 0.5f)
	{
		// 方向キー入力
		charaForwardVec = DirVecR;

		// 進行方向に向けて回転
		charaForwardVec.Normalize();
		owner->RotateToNewForward(charaForwardVec);
	}

	owner->SetComputeWorldTransform();
}
