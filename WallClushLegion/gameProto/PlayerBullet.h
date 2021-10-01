#pragma once

#include "GameObject.h"

class Mesh;
class SkeletalMeshComponent;
class InstanceMeshComponent;

class PlayerBullet :public GameObject
{
public:
	PlayerBullet(const Vector3& pos ,const Vector3& dir,Tag tag);
	~PlayerBullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;
	
private:
	Vector3 mStartPos;				// 初期位置

	bool mIsExist;					// 存在しているかどうか
	unsigned int mQuadVAO;
	unsigned int mQuadVBO;

	float mLiftTime;

	InstanceMeshComponent*  mInstanceMeshComp;
};

