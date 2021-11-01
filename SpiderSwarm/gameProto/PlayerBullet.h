#pragma once

#include "BulletBase.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;
class PlayerBase;
class ObjectPool;


enum class ShotType:unsigned char
{
	SHOT_NORMAL = 0,	// 普通のショット
	SHOT_TURN,			// 旋回ショット
	SHOT_HOMING			// ホーミングショット
};


class PlayerBullet :public BulletBase
{
public:
	PlayerBullet(const Vector3& pos, const Vector3& dir, float scale, float speed);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;

	void SetCollider();

	void NormalMove(float deltaTime);	// 普通の弾道
	void TurnMove(float deltaTime);		// 旋回弾
	void HomingMove(float deltaTime);

private:
	ShotType mShotType;		// ショットの種類

	float mLifeTime;		// 弾の生存時間

	float mTurnShotTime;
	static const float mTurnShotInterval;

	float mVelocityX;	// X方向
	float mVelocityY;	// Y方向
};

