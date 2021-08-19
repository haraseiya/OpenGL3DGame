#pragma once
#include <vector>

class GameObject;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager
{
public:
	EnemyManager(GameObject* target);
	~EnemyManager();

	void CreateFirstWave();
	void CreateSecondWave();
	void Update(float deltaTime);

	bool GetWaveFinishFlag() { return mIsLastWave; }

private:
	GameObject* mTarget;
	std::vector<WeakEnemy*> mWeakEnemy;
	BossEnemy* mBossEnemy;
	StrongEnemy* mStrongEnemy;

	std::vector<std::vector<GameObject*>> mEnemyWaveList;
	std::vector<GameObject*> mFirstWave;
	std::vector<GameObject*> mSecondWave;

	static const int mMaxEnemyNum;
	float mTime;
	int mWaveCount;
	bool mIsLastWave;
	int mEnemyNum;
	bool mIsNext;
};

