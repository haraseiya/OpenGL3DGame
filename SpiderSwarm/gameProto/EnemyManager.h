#pragma once
#include "Singleton.h"
#include <vector>
#include "Math.h"

class GameObject;
class EnemyBase;
class EnemySpawner;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;

class EnemyManager /*: public Singleton<EnemyManager>*/
{
public:
	//friend class Singleton<EnemyManager>;	// Singletonでのインスタンス作成は許可

	EnemyManager(GameObject* target);
	virtual ~EnemyManager();

	void CreateFirstWave();			// 最初のウェーブを作成
	void CreateWave(int waveCount);	// ウェーブの作成
	void RemoveDeadEnemy();			// 死んだ敵を破棄
	void Update(float deltaTime);	// 更新処理

	bool GetWaveFinishFlag() { return mIsLastWave; }	// 終了フラグの取得
	bool GetEnemyExtinction();							// 敵が絶滅したかどうか

	int GetWaveCount() { return mWaveCount; }
	EnemyBase* GetNearestEnemy(std::vector<EnemyBase*> enemys);	// 一番近い敵

private:
	GameObject* mTarget;				// ターゲットするオブジェクト

	EnemyBase* mWeakEnemyPrototype;		
	EnemySpawner* mWeakEnemySpawner;
	EnemyBase* mStrongEnemyPrototype;

	EnemySpawner* mStrongEnemySpawner;
	EnemyBase* mBossEnemyPrototype;
	EnemySpawner* mBossEnemySpawner;

	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// 敵ウェーブリスト
	std::vector<EnemyBase*> mEnemyWave0;					// チュートリアル用ウェーブ
	std::vector<EnemyBase*> mEnemyWave1;					// ウェーブ1
	std::vector<EnemyBase*> mEnemyWave2;					// ウェーブ2
	std::vector<EnemyBase*> mEnemyWave3;					// ウェーブ3
	std::vector<EnemyBase*> mEnemyWave4;					// ウェーブ4

	Vector3 mOffset;					// 敵位置のオフセット

	static const int mMaxEnemyWave;
	static const int mMaxEnemyNum;		// 敵の最大数
	static const int mMaxWeakEnemy;		// 雑魚敵最大数
	static const int mMaxStrongEnemy;	// 強敵最大数
	static const int mMaxBossEnemy;		// ボス敵最大数

	static const float mRandomRangeMinX;
	static const float mRandomRangeMaxX;
	static const float mRandomRangeMinY;
	static const float mRandomRangeMaxY;

	float mTime;						// タイマー
	int mWaveCount;						// ウェーブのカウント
	bool mIsLastWave;					// ラストウェーブか
	int mEnemyNum;						// 現在の敵の数
	bool mIsNext;						// 次のウェーブに行けるか
	bool mIsExtinction;					// 現在のウェーブの敵が絶滅したか
	float mDistance;
};

