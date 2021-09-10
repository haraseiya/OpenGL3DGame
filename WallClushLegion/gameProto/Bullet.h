#pragma once

#include "GameObject.h"

class Mesh;
class ExplosionEffect;
class SkeletalMeshComponent;

class Bullet :public GameObject
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
	Mesh* mMesh;
	ExplosionEffect* mExplosion;	// 爆発エフェクト
	SkeletalMeshComponent* mSkeletalMeshComponent;

	unsigned int mQuadVAO;
	unsigned int mQuadVBO;

	BulletType mBulletType;
};

