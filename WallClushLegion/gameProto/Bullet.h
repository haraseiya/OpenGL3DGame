#pragma once

#include "GameObject.h"

class Mesh;
class ExplosionEffect;
class SkeletalMeshComponent;

class Bullet :public GameObject
{

public:
	// �e�̎��
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
	Vector3 mStartPos;				// �����ʒu
	bool mIsExist;					// ���݂��Ă��邩�ǂ���
	Mesh* mMesh;
	ExplosionEffect* mExplosion;	// �����G�t�F�N�g
	SkeletalMeshComponent* mSkeletalMeshComponent;

	unsigned int mQuadVAO;
	unsigned int mQuadVBO;

	BulletType mBulletType;
};

