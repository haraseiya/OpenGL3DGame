#include "BulletManager.h"
#include "PlayerBullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"
#include "Input.h"
#include "Game.h"

// égÇ§íeÇÃëçêî
const int BulletManager::mAmount = 1000;
const float BulletManager::mInterval = 0.1f;

BulletManager::BulletManager(GameObject* owner)
	: mOwner(owner)
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::Update(float deltaTime)
{
}

void BulletManager::CreateBullet()
{
}

void BulletManager::DeleteBullet()
{
}
