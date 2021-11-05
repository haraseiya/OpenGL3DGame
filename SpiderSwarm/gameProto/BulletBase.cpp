#include "BulletBase.h"
#include "InstanceMeshComponent.h"

BulletBase::BulletBase(Tag tag, InstanceType instanceType)
	: GameObject(tag)
{
	// �C���X�^���X���b�V������
	mInstanceMeshComp = new InstanceMeshComponent(this, instanceType);
}

BulletBase::~BulletBase()
{
}

void BulletBase::Create()
{
	mIsExist = false;

}
