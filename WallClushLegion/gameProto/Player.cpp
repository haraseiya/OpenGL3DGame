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
#include "Bullet.h"

#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttack.h"
#include "PlayerStateRevive.h"

const float cAnimationSpeed = 0.5f;
const float Player::m_range = 10.0f;

Player::Player()
	: GameObject(Tag::Player)
	, mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mShootTimer(0.0f)
{
	// 大きさを100分の1に
	mScale = 0.01f;

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

	// プレイヤーステートプールの初期化
	mStatePools.push_back(new PlayerStateIdle);			// 待機状態
	mStatePools.push_back(new PlayerStateRun);			// 走る状態	
	mStatePools.push_back(new PlayerStateRevive);		// 蘇生状態

	// あたり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this);
	playerBox.mMin.x *= 1.2f;
	playerBox.mMin.y *= 1.2f;
	playerBox.mMax.x *= 1.2f;
	playerBox.mMax.y *= 1.2f;
	mHitBox->SetObjectBox(playerBox);

	//// 詠唱範囲用トリガー
	//AABB playerTriggerBox = mesh->GetCollisionBox();
	//mHitTrigger = new BoxCollider(this);
	//playerTriggerBox.mMin.x *= 5.0f;
	//playerTriggerBox.mMax.x *= 5.0f;
	//playerTriggerBox.mMin.y *= 10.0f;
	//playerTriggerBox.mMax.y *= 10.0f;

	//mHitTrigger->SetObjectBox(playerTriggerBox);
	//mHitTrigger->SetArrowRotate(false);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this);
	mHitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this);
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
	const bool canChangeState = mNowState != mNextState;

	// ステート外部からステート変更があったか？
	if (canChangeState)
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

	// 敵がしないならエイム機能を停止
	if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	{
		mAimMode = false;
		return;
	}
	if (!mAimMode)
	{
		mTarget = GAMEINSTANCE.GetEnemyActor();
	}

	// ターゲットを指定
	Vector3 aimPos, aimDir;
	aimPos = mTarget->GetPosition();

	//自身から敵に向かう向きベクトルを計算
	aimDir = aimPos - mPosition;
	aimDir.z = 0.0f;

	// プレーヤーと十分距離があるなら向きを変更
	if (aimDir.LengthSq() > 0.5f)
	{
		aimDir.Normalize();
		mDirection = aimDir;
	}

	//キーが押された
	const float interval = 0.1f;
	mShootTimer += deltaTime;
	if (mShootTimer > interval&&INPUT_INSTANCE.GetInput(KEY_A)==KEY_STATE_PRESSED)
	{
		mShootTimer = 0.0f;
		Bullet* ba = new Bullet(mPosition, mDirection, Tag::PlayerBullet);
	}

	//if (INPUT_INSTANCE.GetInput(KEY_A) == KEY_STATE_PUSHDOWN)
	//{
	//	ChantEffect* effect = new ChantEffect(this,true);
	//}
}

// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void Player::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* Player::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void Player::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// タグ追加
	Tag colliderTag = other->GetTag();

	// 衝突した物体のタグが背景の場合
	if (colliderTag == Tag::BackGround)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB bgBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, bgBox, fix);

			// 補正ベクトル分戻す
			mPosition += fix;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}

	// 衝突した物体のタグが敵の場合
	if (colliderTag == Tag::Enemy)
	{
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// 補正ベクトル分戻す
			mPosition += fix;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}
}
