#include "BulletBase.h"
#include "InstanceMeshComponent.h"

BulletBase::BulletBase(Tag tag, InstanceType instanceType)
	: GameObject(tag)
{
	//mInstanceMeshComp = new InstanceMeshComponent(this, instanceType);
}

BulletBase::~BulletBase()
{
}
