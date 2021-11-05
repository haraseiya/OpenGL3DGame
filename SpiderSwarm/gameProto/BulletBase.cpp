#include "BulletBase.h"
#include "InstanceMeshComponent.h"

BulletBase::BulletBase(Tag tag, InstanceType instanceType)
	: GameObject(tag)
{
	// インスタンスメッシュ生成
	mInstanceMeshComp = new InstanceMeshComponent(this, instanceType);
}

BulletBase::~BulletBase()
{
}

void BulletBase::Create()
{
	mIsExist = false;

}
