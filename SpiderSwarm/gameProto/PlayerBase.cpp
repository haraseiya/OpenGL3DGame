#include "PlayerBase.h"
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
#include "SceneBase.h"

#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed=0.5f;
const float PlayerBase::mInterval = 0.1f;
const float PlayerBase::mInvincibleTime = 3.0f;
const float PlayerBase::mSpecialShotInterval = 5.0f;

// 状態に変更があったらこちらも変更
const char* playerStateEnumName[static_cast<int>(PlayerStateEnum::StateNum)] =
{
	"PlayerStateEnum::Invalid",
	"PlayerStateEnum::Spawn",
	"PlayerStateEnum::Idle",
	"PlayerStateEnum::Walk",
	"PlayerStateEnum::Turn",
	"PlayerStateEnum::Run",
	"PlayerStateEnum::LookAround"
	"PlayerStateEnum::Attack1",
	"PlayerStateEnum::Attack2",
	"PlayerStateEnum::Attack3",
	"PlayerStateEnum::GetDamage",
	"PlayerStateEnum::Roar",
	"PlayerStateEnum::Stun",
	"PlayerStateEnum::Die",

	"PlayerStateEnum::StateNum"
};

PlayerBase::PlayerBase()
	: GameObject(Tag::PLAYER)
	, mShootTimer(0.0f)
	, mIsDeadAnimFinish(false)
	, mPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE)
{	
	// 大きさを100分の1に
	mScale = 0.01f;
	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	mInvincibleTimer += deltaTime;
	mMeshComp->SetVisible(false);
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

// プレイヤー衝突判定
void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// 当たったオブジェクトのタグ取得
	Tag colliderTag = other->GetTag();

	const bool isHitBullet = colliderTag == Tag::ENEMY_BULLET && mInvincibleTimer >= mInvincibleTime;
	// 敵の弾に当たったらプレイヤーの体力を１減らす
	if (isHitBullet)
	{
		mInvincibleTimer = 0.0f;
		
		mMeshComp->SetVisible(false);

		mHitPoint--;
	}

	// 衝突したのが背景の場合
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
		// 別のタイプがボックスの場合
		if (other->GetColliderType() == ColliderTypeEnum::Box)
		{
			Vector3 fix;

			// 壁とぶつかったとき
			AABB playerBox = mHitBox->GetWorldBox();
			AABB enemyBox = dynamic_cast<BoxCollider*>(other)->GetWorldBox();

			// めり込みを修正
			calcCollisionFixVec(playerBox, enemyBox, fix);

			// 補正ベクトル分戻す
			mPosition = Vector3::Lerp(mPosition,mPosition+fix,0.1);
			mPosition.z = 500.0f;

			// 位置が変わったのでボックス再計算
			ComputeWorldTransform();
		}
	}
}

// プレイヤーのステート名を取得
const char* PlayerBase::GetPlayerStateEnumName(PlayerStateEnum state)
{
	return playerStateEnumName[static_cast<int>(state)];
}

void PlayerBase::SetForwardVec(Vector3& v)
{
	mForwardVec = v;
	RotateToNewForward(v);
}