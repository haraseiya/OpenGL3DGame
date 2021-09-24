#pragma once

#include <vector>

class Bullet;
class GameObject;
class InstanceBase;
class InstanceMeshManager;

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void Update();

private:
	std::vector<InstanceBase*> mBullets;
	InstanceMeshManager* mInstanceMeshManager;

	static const int mAmount;	// オブジェクトの総数
};
