#pragma once

class Bullet;
class GameObject;

class BulletManager
{
public:
	BulletManager(GameObject* owber);
	~BulletManager();

	void Update(float deltaTime);

private:
	static const int mAmount;
	Bullet* mBullet;

	unsigned int mBuffer;

	Bullet mModel;
	Bullet* mModelMatrices;
};

