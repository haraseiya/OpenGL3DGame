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
	Vector3 mStartPos;				// �����ʒu
	bool mIsExist;					// ���݂��Ă��邩�ǂ���
	ExplosionEffect* mExplosion;	// �����G�t�F�N�g
};

