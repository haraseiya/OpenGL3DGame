#include "EnemyManager.h"
#include "WeakEnemy.h"


const int EnemyManager::mMaxEnemyNum = 100;

EnemyManager::EnemyManager(GameObject* target)
{
	// ランダムな位置にエネミーを配置
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mWeakEnemy.push_back(new WeakEnemy(target));
		mWeakEnemy[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

EnemyManager::~EnemyManager()
{
}
