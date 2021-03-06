#pragma once
#include "GameObject.h"
#include "InstanceType.h"

class InstanceMeshComponent;
class InstanceMeshManager;

class BulletBase : public GameObject
{
public:
	BulletBase(Tag tag,InstanceType instanceType);
	~BulletBase();

	virtual void SetCollider() = 0;
	void Create();
protected:
	InstanceMeshComponent* mInstanceMeshComp;
	InstanceMeshManager* mInstanceMeshManager;

	float mVelocityX;	// X向きの速さ
	float mVelocityY;	// Y向きの速さ
	float mYaw;			// 回転角度

	bool mIsExist;		// 生存しているかどうか
};