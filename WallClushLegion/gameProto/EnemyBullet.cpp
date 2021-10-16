#include "EnemyBullet.h"
#include "EnemyBase.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

const float EnemyBullet::mMaxLifeTime = 10.0f;

EnemyBullet::EnemyBullet(EnemyBase* enemy,const Vector3& direction,float scale,float speed)
	: GameObject(Tag::ENEMY_BULLET)
	, mIsUse(true)
{
	// �p�����[�^�[������
	mPosition = enemy->GetPosition();
	mPosition.z = enemy->GetPosition().z + 50;
	mDirection = direction;
	mScale = scale;
	mSpeed = speed;
	mLifeTime = 0.0f;

	// �|�����f���ǂݍ���
	mInstanceMeshComp = new InstanceMeshComponent(this, InstanceType::EnemyBullet1);

	// �e�����蔻��
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::UpdateActor(float deltaTime)
{
	// 
	mLifeTime += deltaTime;

	// �������Ԃ��߂���Ύ��g������
	const bool isDead = mLifeTime >= mMaxLifeTime;
	if (isDead)
	{
		mLifeTime = 0.0f;
		mState = STATE_DEAD;
	}

	mPosition += mSpeed * deltaTime * mDirection;
	mRecomputeWorldTransform = true;
}

void EnemyBullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// �^�O���Q�b�g
	Tag otherTag = otherBox->GetTag();

	if (otherTag == Tag::PLAYER)
	{
		mState = STATE_DEAD;
	}
}
