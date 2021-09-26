#pragma once

#include <vector>

class Bullet;
class GameObject;
class InstanceBase;
class InstanceMeshManager;

class BulletManager
{
public:
	BulletManager(GameObject* owner);
	~BulletManager();

	void Update(float deltaTime);

private:
	std::vector<Bullet*> mBullets;
	InstanceMeshManager* mInstanceMeshManager;
	GameObject* mOwner;

	float mShootTimer;
	static const int mAmount;		// �I�u�W�F�N�g�̑���
	static const float mInterval;	// �V���b�g�̊Ԋu
};