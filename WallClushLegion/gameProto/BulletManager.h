#pragma once

class Bullet;
class GameObject;
class Shader;

class BulletManager
{
public:
	BulletManager(GameObject* owber);
	~BulletManager();

	void Update(float deltaTime);

private:
	static const int mAmount;
	Bullet* mBullet;

	Shader* mBulletShader;
	unsigned int mBuffer;

	Matrix4 mModel;
	Matrix4* mModelMatrices;

	unsigned int mVAO;
};

