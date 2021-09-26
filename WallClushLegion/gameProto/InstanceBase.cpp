#include "InstanceBase.h"
#include "InstanceMeshManager.h"

InstanceBase::InstanceBase(Tag tag,Mesh* mesh)
	: GameObject(tag)
	, mMesh(mesh)
{
}

InstanceBase::~InstanceBase()
{
}
