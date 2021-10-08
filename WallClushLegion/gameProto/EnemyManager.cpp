#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"

const int EnemyManager::mMaxEnemyNum = 30;
const int EnemyManager::mMaxEnemyWave = 3;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTime(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
{
	// 最初のウェーブを生成
	CreateFirstWave();
}

EnemyManager::~EnemyManager()
{
	// 残ったエネミーの削除
	for (auto e : mEnemyWave1)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
	for (auto e : mEnemyWave2)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
	for (auto e : mEnemyWave3)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
}

void EnemyManager::CreateFirstWave()
{
	// 敵ウェーブ1作成
	mEnemyWave1.emplace_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave1[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

void EnemyManager::CreateWave(int waveCount)
{
	switch (waveCount)
	{
	case 1:
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		for (int i = 0; i < mMaxEnemyNum; i++)
		{
			mEnemyWave2.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave2[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		}
		break;
	case 2:
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		for (int i = 0; i < mMaxEnemyNum; i++)
		{
			mEnemyWave3.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave3[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		}
		break;

	default: 
		return;
	}
}

void EnemyManager::RemoveDeadEnemy()
{
}

void EnemyManager::Update(float deltaTime)
{
	mTime += deltaTime;

	// 1体でも生存状態の敵がいたら次のウェーブに行かない
	if (GAMEINSTANCE.IsExistActorType(Tag::ENEMY))
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
		mWaveCount++;

		// 全てのウェーブが終わったらフラグをtrueに
		if (mWaveCount == mMaxEnemyWave)
		{
			mIsLastWave = true;
		}

		// ウェーブ数がリストサイズを超えたら
		if (mWaveCount >= mMaxEnemyWave)
		{
			return;
		}

		// 次ウェーブの敵を生成
		CreateWave(mWaveCount);
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}


