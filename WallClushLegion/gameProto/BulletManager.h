#pragma once

#include <vector>

class GameObject;
class PlayerBullet;
class EnemyBullet;
class InstanceMeshManager;

class BulletManager
{
public:
	BulletManager(GameObject* owner);
	~BulletManager();

	void Update(float deltaTime);

private:
	PlayerBullet* mPlayerBullet;
	InstanceMeshManager* mInstanceMeshManager;
	GameObject* mOwner;

	float mShootTimer;
	static const int mAmount;		// �I�u�W�F�N�g�̑���
	static const float mInterval;	// �V���b�g�̊Ԋu
};