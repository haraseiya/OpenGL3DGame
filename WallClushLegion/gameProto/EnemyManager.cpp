#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"

const int EnemyManager::mMaxEnemyNum = 30;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTime(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
{
	mEnemyWaveList.emplace_back(mEnemyWave);
	// 最初のウェーブを生成
	//CreateFirstWave();
	// 第一陣
	mEnemyWave.emplace_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mEnemyWave.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateFirstWave()
{

}

void EnemyManager::CreateWave()
{
}

void EnemyManager::RemoveDeadEnemy()
{
}

void EnemyManager::Update(float deltaTime)
{
	mTime += deltaTime;

	// 1体でも生存状態の敵がいたら次のウェーブに行かない
	if (GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	{
		mIsNext = false;
	}
	else 
	{ 
		mIsNext = true; 
	}

	// 次のウェーブに移動可能であれば
	if (mIsNext)
	{
		printf("次のウェーブ\n");
		mWaveCount++;

		// 全てのウェーブが終わったらフラグをtrueに
		if (mWaveCount == mEnemyWaveList.size() )
		{
			mIsLastWave = true;
		}

		// ウェーブ数がリストサイズを超えたら
		if (mWaveCount >= mEnemyWaveList.size())
		{
			return;
		}

		// 次ウェーブの敵をアクティブに
		RemoveDeadEnemy();
		CreateWave();
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

