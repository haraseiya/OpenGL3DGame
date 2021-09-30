#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"
#include "Input.h"
#include "Game.h"

// 使う弾の総数
const int BulletManager::mAmount = 1000;
const float BulletManager::mInterval = 0.1f;

BulletManager::BulletManager(GameObject* owner)
	: mOwner(owner)
{
	mInstanceMeshManager = new InstanceMeshManager();
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update(float deltaTime)
{
	Vector3 shotPos1 = mOwner->GetPosition();
	Vector3 shotPos2 = Vector3(mOwner->GetPosition().x+30, mOwner->GetPosition().y + 30, mOwner->GetPosition().z);
	Vector3 shotPos3 = Vector3(mOwner->GetPosition().x-30, mOwner->GetPosition().y - 30, mOwner->GetPosition().z);

	// ショット可能であれば弾を生成
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;
		mBullet = new Bullet(shotPos1, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
		mBullet = new Bullet(shotPos2, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
		mBullet = new Bullet(shotPos3, Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet);
	}
}
