#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"

// g‚¤’e‚Ì‘”
const int BulletManager::mAmount = 1000;

BulletManager::BulletManager()
{
	mInstanceMeshManager = new InstanceMeshManager(mBullets,mAmount);
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update()
{
	mInstanceMeshManager->SetShader();
}
