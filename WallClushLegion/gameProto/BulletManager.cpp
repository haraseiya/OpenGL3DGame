#include "BulletManager.h"
#include "Bullet.h"
#include "Renderer.h"
#include "Shader.h"

// 使う弾の総数
const int BulletManager::mAmount = 100;

BulletManager::BulletManager(GameObject* owner)
{
	mModel = Matrix4::Identity;

	// シェーダー読み込み
	mBulletShader->Load("Shaders/Bullet.vert", "Shaders/Bullet.frag");

	// 使うモデルデータを全て確保
	mModelMatrices = new Matrix4[mAmount];

	// 弾の情報を行列に入れる
	for (int i = 0; i < mAmount; i++)
	{
		//mBullet = new Bullet(owner->GetPosition(),owner->GetForward(),owner->GetTag());
		mModel = Matrix4::CreateTranslation(owner->GetPosition());
		mModel = Matrix4::CreateScale(owner->GetScale());
		mModel = Matrix4::CreateRotationX(0);
		mModelMatrices[i] = mModel;
	}

	// Bufferに
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, mAmount * sizeof(Matrix4),&mModelMatrices[0], GL_STATIC_DRAW);


	for (unsigned int i = 0; i < ; i++)
	{
		glBindVertexArray(mVAO);

		std::size_t vec4Size = sizeof(float) * 4;
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1*vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update(float deltaTime)
{
	mBulletShader->SetActive();
	mBulletShader->SetIntUniform("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);

	for (unsigned int i = 0; i < mAmount; i++)
	{
		glBindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES, , GL_UNSIGNED_INT, 0, mAmount);
		glBindVertexArray(0);
	}
}
