#include "PlayerBullet.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "PlayerBase.h"
#include "ObjectPool.h"

PlayerBullet::PlayerBullet(PlayerBase* player, const Vector3& pos, const Vector3& dir)
	: BulletBase(Tag::PLAYER_BULLET,InstanceType::PlayerBullet1)
{
	// �p�����[�^�[������
	mPosition = player->GetPosition();
	mPosition.z = player->GetPosition().z + 50.0f;
	mDirection = dir;
	mScale = 0.2f;
	mSpeed = 3000.0f;
	mLifeTime = 0.0f;

	// �R���C�_�[�̃Z�b�g
	SetCollider();
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::UpdateActor(float deltaTime)
{
	mLifeTime += deltaTime;

	// �������Ԃ��߂���Ύ��g������
	const bool isDead = mLifeTime >= 3.0f;
	if (isDead)
	{
		mLifeTime = 0.0f;
		mState = STATE_DEAD;
	}

	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void PlayerBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// �^�O���Q�b�g
	Tag otherTag = otherBox->GetTag();

	// �Փ˂����̂��G�̏ꍇ
	if (otherTag == Tag::ENEMY)
	{
		// �e���폜
		mState = STATE_DEAD;
	}
}

void PlayerBullet::SetCollider()
{
	// �e�����蔻��
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}
