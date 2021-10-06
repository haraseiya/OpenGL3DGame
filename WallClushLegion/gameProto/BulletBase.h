#pragma once
#include "GameObject.h"
#include "InstanceType.h"

class InstanceMeshComponent;

class BulletBase : public GameObject
{
public:
	BulletBase(Tag tag,InstanceType instanceType);
	~BulletBase();

protected:
	
};