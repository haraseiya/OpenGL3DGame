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
#include "Bullet.h"
#include "BulletManager.h"

// プレイヤーステート関連
#include "PlayerBehaviorComponent.h"
#include "PlayerStateBase.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"

const float Player1::m_range = 10.0f;

Player1::Player1()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
{
	printf("プレイヤー１作成\n");

	// 弾管理クラス生成
	mBulletManager = new BulletManager(this);

	// プレイヤー情報読み込み
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
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void Player1::UpdateActor(float deltaTime)
{
	mBulletManager->Update(deltaTime);
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

void Player1::OnCollisionEnter(ColliderComponent* own,ColliderComponent* other)
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

void Player1::LoadModel()
{
	//メッシュのロード
	mMesh = RENDERER->GetMesh("Assets/Mesh/Player.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
}

void Player1::LoadSkeleton()
{
	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Skelton/Player.gpskel"));
}

void Player1::LoadAnimation()
{
	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)] = RENDERER->GetAnimation("Assets/Animation/Player_Idle.gpanim", true);
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)] = RENDERER->GetAnimation("Assets/Animation/Player_Running.gpanim", true);
}

void Player1::BehaviorResister()
{
	// プレイヤーステートプールの初期化
	mPlayerBehavior = new PlayerBehaviorComponent(this);
	mPlayerBehavior->RegisterState(new PlayerStateIdle(mPlayerBehavior));
	mPlayerBehavior->RegisterState(new PlayerStateRun(mPlayerBehavior));
	mPlayerBehavior->SetFirstState(PlayerStateEnum::Idle);
}

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
