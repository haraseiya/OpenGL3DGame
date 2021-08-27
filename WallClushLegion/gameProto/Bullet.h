#pragma once

#include "GameObject.h"

class ExplosionEffect;

class Bullet :public GameObject
{
public:
	Bullet(const Vector3& pos ,const Vector3& dir,Tag tag);
	~Bullet();

	void UpdateActor(float deltaTime)override;
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)override;
	
private:
	Vector3 mStartPos;				// 初期位置
	bool mIsExist;					// 存在しているかどうか
	ExplosionEffect* mExplosion;	// 爆発エフェクト
};

