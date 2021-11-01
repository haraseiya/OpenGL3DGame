#pragma once

#include "BulletBase.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;
class PlayerBase;
class ObjectPool;


enum class ShotType:unsigned char
{
	SHOT_NORMAL = 0,	// ���ʂ̃V���b�g
	SHOT_TURN,			// ����V���b�g
	SHOT_HOMING			// �z�[�~���O�V���b�g
};


class PlayerBullet :public BulletBase
{
public:
	PlayerBullet(const Vector3& pos, const Vector3& dir, float scale, float speed);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;

	void SetCollider();

	void NormalMove(float deltaTime);	// ���ʂ̒e��
	void TurnMove(float deltaTime);		// ����e
	void HomingMove(float deltaTime);

private:
	ShotType mShotType;		// �V���b�g�̎��

	float mLifeTime;		// �e�̐�������

	float mTurnShotTime;
	static const float mTurnShotInterval;

	float mVelocityX;	// X����
	float mVelocityY;	// Y����
};

