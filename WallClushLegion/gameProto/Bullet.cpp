#include "Bullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir, Tag tag)
	: GameObject(tag)
	, mStartPos(pos)
{
	// パラメーター初期化
	mPosition = pos;
	mPosition.z = 550;
	mDirection = dir;
	mScale = 0.2f;
	mSpeed = 1000.0f;

	// 板ポリモデル読み込み
	mInstanceMeshComp = new InstanceMeshComponent(this,InstanceType::PlayerBullet);

	// 弾当たり判定

	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

Bullet::~Bullet()
{
}

void Bullet::UpdateActor(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void Bullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	Tag otherTag = otherBox->GetTag();

	// 衝突したのが敵の場合
	if (otherTag == Tag::Enemy)
	{
		// 弾を削除
		mState = STATE_DEAD;
	}
}
