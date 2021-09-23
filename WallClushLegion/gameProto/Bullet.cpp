#include "Bullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "InstanceMeshComponent.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir, Tag tag)
	: GameObject(tag)
	, mStartPos(pos)
	, mMesh(nullptr)
	, mExplosion(nullptr)
{
	// パラメーター初期化
	mPosition = pos;
	mDirection = dir;
	mScale = 1.0f;
	mSpeed = 1000.0f;

	// 板ポリモデル読み込み
	mMesh = RENDERER->GetMesh("assets/Mesh/Bullet.gpmesh");
	mSkeletalMeshComponent = new SkeletalMeshComponent(this);
	mSkeletalMeshComponent->SetMesh(mMesh);

	mInstanceMeshComponent = new InstanceMeshComponent(this, mMesh,1000);

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
