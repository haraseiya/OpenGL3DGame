#pragma once

#include "InstanceBase.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;

class Bullet :public InstanceBase
{
public:
	// ’e‚Ìí—Ş
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
	Vector3 mStartPos;				// ‰ŠúˆÊ’u

	bool mIsExist;					// ‘¶İ‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©
	unsigned int mQuadVAO;
	unsigned int mQuadVBO;

	BulletType mBulletType;
};

