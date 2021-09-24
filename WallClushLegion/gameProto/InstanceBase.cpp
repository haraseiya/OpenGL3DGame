#include "InstanceBase.h"

InstanceBase::InstanceBase(Tag tag)
	: GameObject(tag)
	, mMesh(nullptr)
{
}

InstanceBase::~InstanceBase()
{
}
