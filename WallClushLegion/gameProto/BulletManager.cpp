#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"

const int BulletManager::mAmount = 100;

BulletManager::BulletManager(GameObject* owner)
{
	mModel = Matrix4::Identity;
	mModelMatrices = new Matrix4[mAmount];

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * 100, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// also set instance data
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);

	for (int i = 0; i < mAmount; i++)
	{
		mBullet = new Bullet(owner->GetPosition(),owner->GetForward(),owner->GetTag());
		mModel = Matrix4::CreateTranslation(owner->GetPosition());
		mModel = Matrix4::CreateScale(owner->GetScale());
		mModel = Matrix4::CreateRotationX(0);
		mModelMatrices[i] = mModel;
	}

	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, mAmount * sizeof(int),&mModelMatrices[0], GL_STATIC_DRAW);
}

BulletManager::~BulletManager()
{
}

void BulletManager::Update(float deltaTime)
{
	
}
