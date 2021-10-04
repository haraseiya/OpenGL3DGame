#include "PlayerBullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"

PlayerBullet::PlayerBullet(const Vector3& pos, const Vector3& dir, Tag tag)
	: GameObject(tag)
	, mStartPos(pos)
	, mLiftTime(0.0f)
{
	// パラメーター初期化
	mPosition = pos;
	mPosition.z = 550;
	mDirection = dir;
	mScale = 0.2f;
	mSpeed = 1000.0f;

	// 板ポリモデル読み込み
	mInstanceMeshComp = new InstanceMeshComponent(this,InstanceType::PlayerBullet1);

	// 弾当たり判定
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::UpdateActor(float deltaTime)
{
	mLiftTime += deltaTime;

	// 生存期間を過ぎれば自身を消す
	const bool isDead = mLiftTime > 3.0f;
	if (isDead)
	{
		mLiftTime = 0.0f;
		mState = STATE_DEAD;
	}

	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void PlayerBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// タグをゲット
	Tag otherTag = otherBox->GetTag();

	// 衝突したのが敵の場合
	if (otherTag == Tag::ENEMY)
	{
		// 弾を削除
		mState = STATE_DEAD;
	}
}
