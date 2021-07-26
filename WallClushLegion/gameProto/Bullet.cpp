#include "Bullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir, Tag tag)
	: GameObject(tag)
	, mStartPos(pos)
{
	mPosition = pos;
	mDirection = dir;
	mSpeed = 1000.0f;

	// ”Âƒ|ƒŠ•\Ž¦

	// ’e“–‚½‚è”»’è
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

Bullet::~Bullet()
{
}

void Bullet::UpdateActor(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;

	mRecomputeWorldTransform = true;
}
