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

	void CreateWave();
	void Update(float deltaTime);

	bool GetWaveFinishFlag() { return mIsLastWave; }

private:
	GameObject* mTarget;									// ターゲットするオブジェクト

	std::vector<std::vector<GameObject*>> mEnemyWaveList;	// 敵ウェーブリスト
	std::vector<GameObject*> mFirstWave;					// 第一陣
	std::vector<GameObject*> mSecondWave;					// 第二陣
	std::vector<GameObject*> mThirdWave;					// 第三陣

	static const int mMaxEnemyNum;
	float mTime;
	int mWaveCount;
	bool mIsLastWave;
	int mEnemyNum;
	bool mIsNext;
};

