#include "ExperienceItem.h"
#include "InstanceMeshComponent.h"
#include "ColliderComponent.h"
#include "BoxCollider.h"

ExperienceItem::ExperienceItem(const Vector3& pos)
	: GameObject(Tag::ITEM)
	, mLifeTime(0.0f)
{
	mInstanceMeshComp = new InstanceMeshComponent(this,InstanceType::EnergyCapsule);

	// 初期位置
	mPosition = pos;

	// 弾当たり判定
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

ExperienceItem::~ExperienceItem()
{

}

void ExperienceItem::UpdateActor(float deltaTime)
{
	// 生存期間を過ぎれば自身を消す
	mLifeTime += deltaTime;
	const bool isDead = mLifeTime >= 10.0f;
	if (isDead)
	{
		mLifeTime = 0.0f;
		mState = STATE_DEAD;
	}
}

void ExperienceItem::OnCollisionEnter(ColliderComponent* own, ColliderComponent* other)
{
	// タグをゲット
	Tag otherTag = other->GetTag();

	// 衝突したのがプレイヤーの場合
	if (otherTag == Tag::PLAYER)
	{
		// 自身を削除
		mState = STATE_DEAD;
	}
}
