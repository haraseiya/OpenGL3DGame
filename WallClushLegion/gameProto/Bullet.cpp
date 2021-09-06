#include "Bullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir, Tag tag)
	: GameObject(tag)
	, mStartPos(pos)
	, mMesh(nullptr)
	, mExplosion(nullptr)
{
	mPosition = pos;
	mPosition.z = 530;
	mDirection = dir;
	mScale = 1.0f;
	mSpeed = 1000.0f;

	// ”Âƒ|ƒŠƒ‚ƒfƒ‹“Ç‚İ‚İ
	mMesh = RENDERER->GetMesh("assets/Mesh/Bullet.gpmesh");
	mSkeletalMeshComponent = new SkeletalMeshComponent(this);
	mSkeletalMeshComponent->SetMesh(mMesh);

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
	//glBindVertexArray(mQuadVAO);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void Bullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	Tag otherTag = otherBox->GetTag();

	// Õ“Ë‚µ‚½‚Ì‚ª“G‚Ìê‡
	if (otherTag == Tag::Enemy)
	{
		mState = STATE_DEAD;
		//mExplosion = new ExplosionEffect(mPosition);
	}
}
