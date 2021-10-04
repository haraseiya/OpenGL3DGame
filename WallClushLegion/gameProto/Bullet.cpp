#include "Bullet.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir,Tag tag,float speed,float scale)
	: GameObject(tag)
	, mLifeTime(0.0f)
{
	// パラメーター初期化
	mPosition = pos;
	mPosition.z = 550.0f;
	mDirection = dir;
	mScale = scale;
	mSpeed = speed;
	mTag = tag;

	// 板ポリモデル読み込み
	mInstanceMeshComp = new InstanceMeshComponent(this, InstanceType::EnemyBullet1);

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

void Bullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// タグをゲット
	Tag otherTag = otherBox->GetTag();

	switch (mTag)
	{
	// 自身がプレイヤー弾
	case Tag::PLAYER_BULLET:
		// 敵に衝突した場合破棄
		if (otherTag == Tag::ENEMY)
		{
			mState = STATE_DEAD;
		}
		break;

	// 自身が敵弾の場合
	case Tag::ENEMY_BULLET:
		// 衝突したのが敵の場合
		if (otherTag == Tag::PLAYER)
		{
			// 弾を削除
			mState = STATE_DEAD;
		}
		break;
	}
}
