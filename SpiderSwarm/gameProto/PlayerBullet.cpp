#include "PlayerBullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "PlayerBase.h"
#include "ObjectPool.h"

PlayerBullet::PlayerBullet(PlayerBase* player, const Vector3& pos, const Vector3& dir)
	: BulletBase(Tag::PLAYER_BULLET,InstanceType::PlayerBullet1)
{
	// パラメーター初期化
	mPosition = player->GetPosition();
	mPosition.z = player->GetPosition().z + 50.0f;
	mDirection = dir;
	mScale = 0.2f;
	mSpeed = 3000.0f;
	mLifeTime = 0.0f;

	// コライダーのセット
	SetCollider();
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::UpdateActor(float deltaTime)
{
	mLifeTime += deltaTime;

	// 生存期間を過ぎれば自身を消す
	const bool isDead = mLifeTime >= 3.0f;
	if (isDead)
	{
		mLifeTime = 0.0f;
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

void PlayerBullet::SetCollider()
{
	// 弾当たり判定
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}
