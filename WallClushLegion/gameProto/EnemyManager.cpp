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
	// ‘æˆêw
	mFirstWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mFirstWave.push_back(new WeakEnemy(mTarget));
		mFirstWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
	//// ‘æ“ñw
	//mSecondWave.push_back(new StrongEnemy(mTarget));
	//for (int i = 0; i < mMaxEnemyNum; i++)
	//{
	//	mSecondWave.push_back(new WeakEnemy(mTarget));
	//	mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	//}
	//// ‘æOw
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

	//// “G‚ª‘¶İ‚µ‚È‚¯‚ê‚ÎI—¹ƒtƒ‰ƒO‚ğ—§‚Ä‚é
	//if (!GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	//{
	//	mIsNext = true;
	//}

	////mIsNext = true;

	//if (mIsNext)
	//{
	//	mWaveCount++;

	//	if (mWaveCount == mEnemyWaveList.size() - 1)
	//	{
	//		//mIsLastWave = true;
	//	}
	//	if (mWaveCount >= mEnemyWaveList.size()) return;

	//	CreateWave();
	//}
	//for (auto w : mEnemyWaveList[mWaveCount])
	//{
	//	if (GAMEINSTANCE.IsExistActorType(Tag::Enemy))
	//	{
	//		mIsNext = false;
	//	}
	//}

	//if (mIsNext)
	//{
	//	mWaveCount++;

	//	if (mWaveCount == mEnemyWaveList.size() - 1)
	//	{
	//		mIsLastWave = true;
	//	}
	//	if (mWaveCount >= mEnemyWaveList.size())return;

	//	for (auto enemy : mEnemyWaveList[mWaveCount])
	//	{
	//		
	//	}
	//}
	//if (mTime > 10.0f)
	//{
	//	CreateWave();
	//	mTime = 0.0f;
	//	mWaveCount++;
	//}

	//if (mWaveCount >= 3)
	//{
	//	mIsLastWave=true;
	//}
}

