#pragma once
#include <vector>

class GameObject;
class EnemyBase;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager
{
public:
	EnemyManager(GameObject* target);
	~EnemyManager();

	void CreateFirstWave();			// 最初のウェーブを作成
	void CreateWave(int waveCount);	// ウェーブの作成
	void RemoveDeadEnemy();			// 死んだ敵を破棄
	void Update(float deltaTime);	// 更新処理

	bool GetWaveFinishFlag() { return mIsLastWave; }	// 終了フラグの取得
	bool GetEnemyExtinction();							// 敵のが絶滅したかどうか

	int GetWaveCount() { return mWaveCount; }

private:
	GameObject* mTarget;	// ターゲットするオブジェクト

	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// 敵ウェーブリスト
	std::vector<EnemyBase*> mEnemyWave1;					// ウェーブ1
	std::vector<EnemyBase*> mEnemyWave2;					// ウェーブ2
	std::vector<EnemyBase*> mEnemyWave3;					// ウェーブ3
	std::vector<EnemyBase*> mEnemyWave4;					// ウェーブ4

	static const int mMaxEnemyWave;
	static const int mMaxEnemyNum;		// 敵の最大数
	static const int mMaxWeakEnemy;		// 雑魚敵最大数
	static const int mMaxStrongEnemy;	// 強敵最大数
	static const int mMaxBossEnemy;		// ボス敵最大数

	float mTime;						// タイマー
	int mWaveCount;						// ウェーブのカウント
	bool mIsLastWave;					// ラストウェーブか
	int mEnemyNum;						// 現在の敵の数
	bool mIsNext;						// 次のウェーブに行けるか
	bool mIsExtinction;					// 現在のウェーブの敵が絶滅したか
	float mDistance;
};

