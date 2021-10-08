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

const float Player1::m_range = 10.0f;
const float Player1::mSpecialShotInterval = 5.0f;

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mBullet(nullptr)
{
	mHitPoint = 100;
	
	printf("プレイヤー１作成\n");

	// リソースの読み込み
	LoadResource();

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
	mShootTimer += deltaTime;
	mSpecialShotTimer += deltaTime;

	// 撃てる条件がそろっていれば
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;
		mBullet = new PlayerBullet(this);
		//mBullet = new Bullet(shotPos2, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
		//mBullet = new Bullet(shotPos3, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	}

	// スペシャルショットが撃てるなら
	const bool isSpecialShot= INPUT_INSTANCE.IsKeyPressed(KEY_Y) && mSpecialShotTimer > mSpecialShotInterval;
	if (isSpecialShot)
	{
		mSpecialShotTimer = 0.0f;
		mLaser = new LaserEffect(this);
	}
}

// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void Player1::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* Player1::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* Player1::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// 当たり判定の入り
void Player1::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
{
	// タグ追加
	Tag colliderTag = other->GetTag();

	if (colliderTag == Tag::ENEMY_BULLET)
	{
		mHitPoint--;
	}

	// 衝突した物体のタグが背景の場合
	if (colliderTag == Tag::BACK_GROUND)
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
	if (colliderTag == Tag::ENEMY)
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

// リソースの読み込み
void Player1::LoadResource()
{
	LoadModel();
	LoadSkeleton();
	LoadAnimation();
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
	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("assets/Animation/Player1_Idle.gpanim", false);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN_FORWARD)] = RENDERER->GetAnimation("assets/Animation/Player1_Forward.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_DIE)] = RENDERER->GetAnimation("assets/Animation/Player_Die2.gpanim", false);
	//mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_VICTORY)] = RENDERER->GetAnimation("assets/Animation/Player_Salute.gpanim", false);
	//mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM)] = RENDERER->GetAnimation("assets/Animation/Player1_Forward.gpanim", true);
}

// ふるまいの登録
void Player1::BehaviorResister()
{
	// プレイヤーステートプールの初期化
	mPlayerBehavior = new PlayerBehaviorComponent(this);
	mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateRunForward(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateDie(mPlayerBehavior));
	mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
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
