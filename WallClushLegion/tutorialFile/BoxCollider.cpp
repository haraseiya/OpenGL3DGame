#include "BoxCollider.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "Actor.h"

// BoxColliderコンストラクタ
BoxCollider::BoxCollider(Actor * owner, PhysicsWorld::EnumPhysicsType physicsType, int updateOrder)
	:Component(owner, updateOrder)
	,mObjectBox(Vector3::Zero, Vector3::Zero)                    // 初期値は大きさなしのボックスに
	,mWorldBox(Vector3::Zero, Vector3::Zero)
	,mRotatable(true)
{
	GAMEINSTANCE.GetPhysics()->AddBoxCollider(physicsType, this); // ボックスコライダーをPhysicsWirldに登録
}

BoxCollider::~BoxCollider()
{
	printf("remove BoxCollider : [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetPhysics()->RemoveBoxCollider(this);
}

void BoxCollider::OnUpdateWorldTransform()
{
	// オブジェクト空間のボックスにリセット
	mWorldBox = mObjectBox;

	// スケーリング
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	// 回転
	if (mRotatable)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	//平行移動
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}
