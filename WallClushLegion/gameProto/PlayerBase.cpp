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

#include "PlayerStateBase.h"
#include "PlayerStateRunForward.h"
#include "PlayerStateIdle.h"

const float PlayerBase::mAnimationSpeed=0.5f;
const float PlayerBase::mInterval = 0.1f;

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
	, mScenePlayer(PlayerSceneState::PLAYER_TITLESCENE)
	, mShootTimer(0.0f)
	, mIsDeadAnimFinish(false)
{	
	// 大きさを100分の1に
	mScale = 0.01f;
	mHitPoint = 10;
	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
}

PlayerBase::~PlayerBase()
{
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerBase::UpdateActor(float deltaTime)
{
	// ショット可能であれば弾を生成
	//mShootTimer += deltaTime;
	//const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	//if (isShoot)
	//{
	//	mShootTimer = 0.0f;
	//	mPlayerBullet = new PlayerBullet(shotPos1, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PLAYER_BULLET);
	//	//mBullet = new Bullet(shotPos2, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	//	//mBullet = new Bullet(shotPos3, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	//}
	//// ステートチェンジ可能であればtrue
	//const bool canChangeState = mNowState != mNextState;

	//// ステート外部からステート変更があったか？
	//if (canChangeState)
	//{
	//	mStatePools[static_cast<unsigned int>(mNowState)]->OnExit();
	//	mStatePools[static_cast<unsigned int>(mNextState)]->OnEnter();
	//	mNowState = mNextState;
	//	return;
	//}

	//// ステート実行
	//mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(deltaTime);

	//// ステート内部からステート変更あったか？
	//if (mNowState != mNextState)
	//{
	//	mStatePools[static_cast<unsigned int>(mNowState)]->OnExit();
	//	mStatePools[static_cast<unsigned int>(mNextState)]->OnEnter();
	//	mNowState = mNextState;
	//}

	//// 敵が存在しないならAimモード停止
	//if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	//{
	//	mAimMode = false;
	//	return;
	//}
	//if (!mAimMode)
	//{
	//	mTarget = GAMEINSTANCE.GetEnemyActor();
	//}

	//// ターゲットを指定
	//Vector3 aimPos, aimDir;
	//aimPos = mTarget->GetPosition();

	////自身から敵に向かう向きベクトルを計算
	//aimDir = aimPos - mPosition;
	//aimDir.z = 0.0f;

	//// プレーヤーと十分距離があるなら向きを変更
	//if (aimDir.LengthSq() > 0.5f)
	//{
	//	aimDir.Normalize();
	//	//mDirection = aimDir;
	//}
}

// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void PlayerBase::FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox)
{
}

SkeletalMeshComponent* PlayerBase::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerBase::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerBase::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// タグ追加
	Tag colliderTag = other->GetTag();

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

const char* PlayerBase::GetPlayerStateEnumName(PlayerStateEnum state)
{
	return playerStateEnumName[static_cast<int>(state)];
}
