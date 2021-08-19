#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"


const int EnemyManager::mMaxEnemyNum = 100;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTime(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(true)
{
	mEnemyWaveList.push_back(mFirstWave);
	mEnemyWaveList.push_back(mSecondWave);
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateFirstWave()
{
	// �{�X�G�̃C���X�^���X����
	mFirstWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < 10; i++)
	{
		mFirstWave.push_back(new WeakEnemy(mTarget));
	}
	for (auto f : mFirstWave)
	{
		f->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

void EnemyManager::CreateSecondWave()
{
	mSecondWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < 10; i++)
	{
		mSecondWave.push_back(new WeakEnemy(mTarget));
	}
	for (auto f : mSecondWave)
	{
		f->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

void EnemyManager::Update(float deltaTime)
{
	mTime += deltaTime;

	for (auto w : mEnemyWaveList[mWaveCount])
	{
		if (GAMEINSTANCE.IsExistActorType(Tag::Enemy))
		{
			mIsNext = false;
		}
	}

	if (mIsNext)
	{
		mWaveCount++;

		if (mWaveCount == mEnemyWaveList.size() - 1)
		{
			mIsLastWave = true;
		}
		if (mWaveCount >= mEnemyWaveList.size())return;

		for (auto enemy : mEnemyWaveList[mWaveCount])
		{
			
		}
	}
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

