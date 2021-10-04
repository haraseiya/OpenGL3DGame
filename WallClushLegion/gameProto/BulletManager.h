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
	static const int mAmount;		// オブジェクトの総数
	static const float mInterval;	// ショットの間隔
};