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
	// 敵のウェーブリストにウェーブを追加
	mEnemyWaveList.push_back(mFirstWave);
	mEnemyWaveList.push_back(mSecondWave);
	mEnemyWaveList.push_back(mThirdWave);

	mEnemyWaveList.reserve(mEnemyWaveList.size());

	// 最初のウェーブを生成
	CreateFirstWave();
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateFirstWave()
{
	// 第一陣
	mFirstWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mFirstWave.push_back(new WeakEnemy(mTarget));
		mFirstWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

void EnemyManager::CreateWave()
{
	// 第二陣
	mSecondWave.push_back(new StrongEnemy(mTarget));

	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mSecondWave.push_back(new WeakEnemy(mTarget));
		mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}

	// 第三陣
	mThirdWave.push_back(new StrongEnemy(mTarget));

	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mThirdWave.push_back(new WeakEnemy(mTarget));
		mThirdWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
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
		if (mWaveCount == mEnemyWaveList.size() - 1)
		{
			mIsLastWave = true;
		}

		// ウェーブ数がリストサイズを超えたら
		if (mWaveCount >= mEnemyWaveList.size()) return;

		// 次のウェーブリストを描画
		for (auto wave : mEnemyWaveList[mWaveCount])
		{

		}
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

