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
	void CreateWave();
	void Update(float deltaTime);

	bool GetWaveFinishFlag() { return mIsLastWave; }

private:
	GameObject* mTarget;									// ターゲットするオブジェクト

	std::vector<std::vector<GameObject*>> mEnemyWaveList;	// 敵ウェーブリスト
	std::vector<GameObject*> mFirstWave;					// 第一陣
	std::vector<GameObject*> mSecondWave;					// 第二陣
	std::vector<GameObject*> mThirdWave;					// 第三陣

	static const int mMaxEnemyNum;				// 敵の最大数
	float mTime;								// タイマー
	int mWaveCount;								// ウェーブのカウント
	bool mIsLastWave;							// ラストウェーブか
	int mEnemyNum;								// 現在の敵の数
	bool mIsNext;								// 次のウェーブに行けるか
};

