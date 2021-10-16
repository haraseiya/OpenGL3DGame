#pragma once

#include <vector>
#include <memory>

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

	void CreateBullet();	// ����
	void DeleteBullet();	// �폜

private:
	GameObject* mOwner;
	std::unique_ptr<PlayerBullet*> mPlayerBullet;	// �v���C���[�e
	std::unique_ptr<EnemyBullet*> mEnemyBullet;		// �G�e

	InstanceMeshManager* mInstanceMeshManager;

	float mShootTimer;
	static const int mAmount;		// �I�u�W�F�N�g�̑���
	static const float mInterval;	// �V���b�g�̊Ԋu
};