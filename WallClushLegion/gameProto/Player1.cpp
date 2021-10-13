#include "Player1.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"
#include "ChantEffect.h"
#include "PlayerBullet.h"
#include "BulletManager.h"
#include "LaserEffect.h"

// プレイヤーステート関連
#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDie.h"

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mBullet(nullptr)
{
	printf("プレイヤー１作成\n");

	mHitPoint = 100;

	// プレイヤーステートプールの初期化
	mPlayerBehavior = new PlayerBehaviorComponent(this);

	// リソースの読み込み
	LoadModel();
	LoadSkeleton();
	LoadAnimation();

	// ふるまいを追加
	BehaviorResister();

	// あたり判定セット
	SetCollider();
}

Player1::~Player1()
{
	//アニメーション本体の消去はレンダラー側で行われる
	mAnimTypes.clear(); 
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void Player1::UpdateActor(float deltaTime)
{
	// 弾が撃てるなら
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;
		mBullet = new PlayerBullet(this);
	}

	// スペシャルショットが撃てるなら
	mSpecialShotTimer += deltaTime;
	const bool isSpecialShot= INPUT_INSTANCE.IsKeyPressed(KEY_Y) && mSpecialShotTimer > mSpecialShotInterval;
	if (isSpecialShot)
	{
		mSpecialShotTimer = 0.0f;
		mLaser = new LaserEffect(this);
	}
}

SkeletalMeshComponent* Player1::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player1::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// モデルのロード
void Player1::LoadModel()
{
	//メッシュのロード
	mMesh = RENDERER->GetMesh("assets/Mesh/Player1.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

// スケルトンのロード
void Player1::LoadSkeleton()
{
	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/Player1.gpskel"));
}

// アニメーションのロード
void Player1::LoadAnimation()
{
	// アニメーション総数分のメモリを確保
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));

	// シーン毎のアニメーション読み込み
	switch (mPlayerSceneState)
	{
	// タイトルシーン
	case PlayerSceneState::PLAYER_TITLESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("assets/Animation/Player1_Idle.gpanim", true);
		mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
		break;

	// セレクトシーン
	case PlayerSceneState::PLAYER_SELECTSCENE:
		// 選択中のアニメーション

		// 選択後のアニメーション

		break;

	// ゲームシーン
	case PlayerSceneState::PLAYER_GAMESCENE:
		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("assets/Animation/Player1_Forward.gpanim", true);
		// 残り3方向分のアニメーション

		// 4方向分の走りアニメーション

		mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("assets/Animation/Player_Die2.gpanim", false);
		mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
		mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
		mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
		break;

	// リザルトシーン
	case PlayerSceneState::PLAYER_RESULT:
		// 勝利アニメーション
		break;
	}
}

// ふるまいの登録
void Player1::BehaviorResister()
{
}

// 自身のコライダーのセット
void Player1::SetCollider()
{
	// あたり判定セット
	mPlayerBox = mMesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	mPlayerBox.mMin.x *= 1.2f;
	mPlayerBox.mMin.y *= 1.2f;
	mPlayerBox.mMax.x *= 1.2f;
	mPlayerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(mPlayerBox);
}
