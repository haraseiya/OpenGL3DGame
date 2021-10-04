#include "Bullet.h"
#include "InstanceMeshComponent.h"
#include "BoxCollider.h"

Bullet::Bullet(const Vector3& pos, const Vector3& dir,Tag tag,float speed,float scale)
	: GameObject(tag)
	, mLifeTime(0.0f)
{
	// �p�����[�^�[������
	mPosition = pos;
	mPosition.z = 550.0f;
	mDirection = dir;
	mScale = scale;
	mSpeed = speed;
	mTag = tag;

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

Bullet::~Bullet()
{
}

void Bullet::UpdateActor(float deltaTime)
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

void Bullet::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
	// �^�O���Q�b�g
	Tag otherTag = otherBox->GetTag();

	switch (mTag)
	{
	// ���g���v���C���[�e
	case Tag::PLAYER_BULLET:
		// �G�ɏՓ˂����ꍇ�j��
		if (otherTag == Tag::ENEMY)
		{
			mState = STATE_DEAD;
		}
		break;

	// ���g���G�e�̏ꍇ
	case Tag::ENEMY_BULLET:
		// �Փ˂����̂��G�̏ꍇ
		if (otherTag == Tag::PLAYER)
		{
			// �e���폜
			mState = STATE_DEAD;
		}
		break;
	}
}
