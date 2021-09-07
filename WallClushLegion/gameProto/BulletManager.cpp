#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"

const int BulletManager::mAmount = 100;

BulletManager::BulletManager(GameObject* owner)
{
	mModel = Matrix4::Identity;

	// 使うモデルデータを全て確保
	mModelMatrices = new Bullet(owner->GetPosition(),owner->GetForward(),owner->GetTag())/*[mAmount]*/;

	for (int i = 0; i < mAmount; i++)
	{
		mBullet = new Bullet(owner->GetPosition(),owner->GetForward(),owner->GetTag());
		mModel = Matrix4::CreateTranslation(owner->GetPosition());
		mModel = Matrix4::CreateScale(owner->GetScale());
		mModel = Matrix4::CreateRotationX(0);
		mModelMatrices[i] = mModel;
	}

	// Bufferに
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, mAmount * sizeof(int),&mModelMatrices[0], GL_STATIC_DRAW);
}

BulletManager::~BulletManager()
{
}

void BulletManager::Update(float deltaTime)
{
	for (unsigned int i = 0; i < mAmount; i++)
	{
		
	}
}
