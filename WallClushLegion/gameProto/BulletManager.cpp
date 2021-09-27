#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"
#include "Input.h"
#include "Game.h"

// Žg‚¤’e‚Ì‘”
const int BulletManager::mAmount = 1000;
const float BulletManager::mInterval = 0.1f;

BulletManager::BulletManager(GameObject* owner)
	: mOwner(owner)
{
	//if (mBullets.empty())return;
	mInstanceMeshManager = new InstanceMeshManager();
	//mInstanceMeshManager->PreparationBufferMatrices(mBullets);
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update(float deltaTime)
{
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;
		mBullets.emplace_back(new Bullet(mOwner->GetPosition(), Vector3::Transform(Vector3::UnitX, mOwner->GetRotation()), Tag::PlayerBullet));
		//mInstanceMeshManager->PreparationBufferMatrices(mBullets);
	}
}
