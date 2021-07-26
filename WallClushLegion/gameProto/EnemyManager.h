#pragma once
#include <vector>

class GameObject;
class WeakEnemy;
class BossEnemy;

class EnemyManager
{
public:
	EnemyManager(GameObject* target);
	~EnemyManager();

private:
	std::vector<WeakEnemy*> mWeakEnemy;
	BossEnemy* mBossEnemy;

	static const int mMaxEnemyNum;
};

