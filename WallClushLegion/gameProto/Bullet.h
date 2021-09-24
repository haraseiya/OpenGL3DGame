#pragma once

#include "InstanceBase.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;

class Bullet :public InstanceBase
{
public:
	// 弾の種類
	enum class BulletType
	{
		PLAYER_BULLET0 = 0,
		PLAYER_BULLET1,
	};

	Bullet(const Vector3& pos ,const Vector3& dir,Tag tag);
	~Bullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;
	
private:
	Vector3 mStartPos;				// 初期位置

	bool mIsExist;					// 存在しているかどうか
	unsigned int mQuadVAO;
	unsigned int mQuadVBO;

	BulletType mBulletType;
};

