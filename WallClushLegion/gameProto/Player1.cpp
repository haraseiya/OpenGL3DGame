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
const float Player::mInterval = 0.1f;

Player::Player()
	: GameObject(Tag::Player)
	, mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mShootTimer(0.0f)
{

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

	// 敵が存在しないならAimモード停止
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
		//mDirection = aimDir;
	}

	// 弾が撃てるのであれば
	mShootTimer += deltaTime;
	const bool isShot = mShootTimer > mInterval && INPUT_INSTANCE.GetInput(KEY_R) == KEY_STATE_PRESSED;
	if (isShot)
	{
		mShootTimer = 0.0f;
		Bullet* ba = new Bullet(mPosition, this->GetForward(), Tag::PlayerBullet);
	}
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
