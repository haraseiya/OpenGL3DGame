#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"

const int EnemyManager::mMaxEnemyNum = 50;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTime(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(true)
{
	// 敵のウェーブリストにウェーブを追加
	mEnemyWaveList.push_back(mFirstWave);
	mEnemyWaveList.push_back(mSecondWave);
	mEnemyWaveList.push_back(mThirdWave);
	CreateWave();
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateWave()
{
	// 第一陣
	mFirstWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mFirstWave.push_back(new WeakEnemy(mTarget));
		mFirstWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
	//// 第二陣
	//mSecondWave.push_back(new StrongEnemy(mTarget));
	//for (int i = 0; i < mMaxEnemyNum; i++)
	//{
	//	mSecondWave.push_back(new WeakEnemy(mTarget));
	//	mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	//}
	//// 第三陣
	//mSecondWave.push_back(new StrongEnemy(mTarget));
	//for (int i = 0; i < mMaxEnemyNum; i++)
	//{
	//	mSecondWave.push_back(new WeakEnemy(mTarget));
	//	mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	//}
}

void EnemyManager::Update(float deltaTime)
{
	mTime += deltaTime;

	// 1体でもアクティブ状態の敵がいたら次のウェーブに行かない
	for (auto enemy : mEnemyWaveList[mWaveCount])
	{
		if(enemy->GetState()==);
	}

	if (mIsNext)
	{
		mWaveCount++;

		if (mWaveCount == mEnemyWaveList.size() - 1)
		{
			mIsLastWave = true;
		}

		if (mWaveCount >= mEnemyWaveList.size()) return;

		// 次ウェーブの敵をアクティブ化
		for (auto enemy : mEnemyWaveList[mWaveCount])
		{
		}
	}
}

