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

	// �e���f�����̓G�t�F�N�g
	EffectComponent* ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"assets/Effect/MAGICALxSPIRAL/MagicArea.efk");

	// �e�����蔻��
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
