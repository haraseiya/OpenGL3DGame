#include "PlayerBullet.h"
#include "Game.h"
#include "BoxCollider.h"
#include "EffectComponent.h"
#include "ExplosionEffect.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "PlayerBase.h"
#include "ObjectPool.h"

const float PlayerBullet::mTurnShotInterval = 0.5f;

PlayerBullet::PlayerBullet(const Vector3& pos, const Vector3& dir, float scale, float speed)
	: BulletBase(Tag::PLAYER_BULLET,InstanceType::PlayerBullet1)
	, mShotType(ShotType::TURN_SHOT)
{
	// �p�����[�^�[������
	mPosition = pos;
	mPosition.z = pos.z + 100.0f;
	mDirection = dir;
	mScale = scale;
	mSpeed = speed;
	mLifeTime = 0.0f;

	mVelocityX = mSpeed * Math::Cos(Math::Pi * 2 * mDirection.x);
	mVelocityY = mSpeed * Math::Sin(Math::Pi * 2 * mDirection.y);

	// �R���C�_�[�̃Z�b�g
	SetCollider();
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::UpdateActor(float deltaTime)
{
	// �������Ԃ��߂���Ύ��g������
	mLifeTime += deltaTime;
	const bool isDead = mLifeTime >= 3.0f;
	if (isDead)
	{
		mLifeTime = 0.0f;
		mState = STATE_DEAD;
	}

	// �V���b�g�^�C�v�ɂ���ԑJ��
	switch (mShotType)
	{
	// �����V���b�g
	case ShotType::NORMAL_SHOT:
		NormalMove(deltaTime);
		break;

	// ����V���b�g
	case ShotType::TURN_SHOT:
		TurnMove(deltaTime);
		break;
	}

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

void PlayerBullet::NormalMove(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void PlayerBullet::TurnMove(float deltaTime)
{
	mTurnShotTime += deltaTime;
	// ���g�̈ʒu���X�V
	GameObject* target = GAMEINSTANCE.GetEnemyActor();
	//Vector3 direction = target->GetPosition() - mPosition;
	//direction.Normalize();
	//mPosition += 1000 * deltaTime * direction;

	mPosition.x += mVelocityX * deltaTime;
	mPosition.y += mVelocityY * deltaTime;
	mPosition.z = 500.0f;

	const bool isShot = mTurnShotTime > mTurnShotInterval;
	if (isShot)
	{
		mTurnShotTime = 0.0f;
		float rad = Math::Atan2(target->GetPosition().y - mPosition.y, target->GetPosition().x - mPosition.x);
		mVelocityX = mSpeed * Math::Cos(rad);
		mVelocityY = mSpeed * Math::Sin(rad);
	}
	mRecomputeWorldTransform = true;
}
