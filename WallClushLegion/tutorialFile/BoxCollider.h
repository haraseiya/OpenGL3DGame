#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"

class BoxCollider : public Component
{
public:
	BoxCollider(class Actor* owner, PhysicsWorld::EnumPhysicsType physicsType,int updateOrder = 100);
	~BoxCollider();
	void OnUpdateWorldTransform() override;                       // ワールド変換時
	void SetObjectBox(const AABB& box) { mObjectBox = box; }      // あたり判定用境界ボックスをセット
	const AABB& GetWorldBox() const { return mWorldBox; }         // ワールド空間上での境界ボックスを取得
	void  SetArrowRotate(bool value) { mRotatable = value; }      // 回転を許可するか？
private:
	AABB mObjectBox;                                              // オブジェクト空間（変換前）のボックス
	AABB mWorldBox;                                               // ワールド空間に置いた時のボックス
	bool mRotatable;                                              // 回転を許可するか？

	friend class PhysicsWorld;                                    
};