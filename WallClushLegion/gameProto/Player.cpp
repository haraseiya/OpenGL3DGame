#include "Player.h"
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

#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"

const float cAnimationSpeed = 0.5f;
const float Player::m_range = 10.0f;

Player::Player()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
{
	// 大きさを100分の1に
	mScale = Vector3(0.01f,0.01f,0.01f);

	//メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Assets/Mesh/Player.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Player.gpskel"));

	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Animation/Player_Idle.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)] = RENDERER->GetAnimation("Assets/Animation/Player_Running.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_REVIVE)] = RENDERER->GetAnimation("Assets/Animation/Player_Revive.gpanim", true);

	// アイドル状態アニメーションをセット
	mMeshComp->PlayAnimation(mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// アクターステートプールの初期化
	mStatePools.push_back(new PlayerStateIdle);			// 待機状態
	mStatePools.push_back(new PlayerStateRun);			// 走る状態	

	// あたり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	// あたり判定セット
	AABB playerTriggerBox = mesh->GetCollisionBox();
	mHitTrigger = new BoxCollider(this, EnumPhysicsType::EnumPlayerTrigger);
	playerTriggerBox.mMin.x *= 10.0f;
	playerTriggerBox.mMin.y *= 10.0f;
	playerTriggerBox.mMax.x *= 10.0f;
	playerTriggerBox.mMax.y *= 10.0f;
	mHitTrigger->SetObjectBox(playerTriggerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitHeadBox->SetObjectBox(headBox);

	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
}

Player::~Player()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void Player::UpdateActor(float deltaTime)
{
	// ステート外部からステート変更があったか？
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部からステート変更あったか？
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	//キーが押された
	if (INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PUSHDOWN)
	{
		mChantEffect = new ChantEffect(this);
	}
}

// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void Player::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
	Vector3 fix;

	// 壁とぶつかったとき
	AABB bgBox = hitBox->GetWorldBox();
	AABB playerBox = mHitBox->GetWorldBox();

	// めり込みを修正
	calcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	mPosition += fix;

	// 位置が変わったのでボックス再計算
	mHitBox->OnUpdateWorldTransform();
}

SkeletalMeshComponent* Player::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}
