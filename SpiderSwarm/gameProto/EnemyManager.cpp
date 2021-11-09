#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"
#include "EnemySpawnerEffect.h"
#include <iostream>

//EnemyManager* EnemyManager::mInstance = nullptr;// シングルトン生成

const int EnemyManager::mMaxEnemyNum = 30;			// 敵全体の最大数
const int EnemyManager::mMaxBossEnemy = 1;			// ボス敵最大数
const int EnemyManager::mMaxStrongEnemy = 4;		// 強敵最大数
const int EnemyManager::mMaxWeakEnemy = 30;			// 雑魚敵最大数
const float EnemyManager::mSpawnCoolTime = 10.0f;	// スポーンする間隔

const float EnemyManager::mRandomRangeMinX = -2000;
const float EnemyManager::mRandomRangeMaxX = 2000;
const float EnemyManager::mRandomRangeMinY = -2000;
const float EnemyManager::mRandomRangeMaxY = 2000;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTimer(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
	, mCount(0)
{
	mOffset = Vector3(0, 0, 750);

	// サイズ確保
	mEnemySpawnerEffect[0] = new EnemySpawnerEffect(Vector3(2000, 2000, 750));
	mEnemySpawnerEffect[1] = new EnemySpawnerEffect(Vector3(-2000, 2000, 750));
	mEnemySpawnerEffect[2] = new EnemySpawnerEffect(Vector3(-2000, -2000, 750));
	mEnemySpawnerEffect[3] = new EnemySpawnerEffect(Vector3(2000, -2000, 750));

	// 敵を生成
	CreateEnemys();

	// プロトタイプパターン
	//mWeakEnemyPrototype = new WeakEnemy(mTarget);
	//mWeakEnemySpawner = new EnemySpawner(mWeakEnemyPrototype);
	//mStrongEnemyPrototype = new StrongEnemy(mTarget);
	//mStrongEnemySpawner = new EnemySpawner(mStrongEnemyPrototype);
	//mBossEnemyPrototype = new StrongEnemy(mTarget);
	//mBossEnemySpawner = new EnemySpawner(mBossEnemyPrototype);
}

EnemyManager::~EnemyManager()
{
	// 残ったエネミーの削除
	//for (auto e : mEnemyWave1)
	//{
	//	if (e != nullptr)
	//	{
	//		e->SetState(GameObject::STATE_DEAD);
	//	}
	//}
	//for (auto e : mEnemyWave2)
	//{
	//	if (e != nullptr)
	//	{
	//		e->SetState(GameObject::STATE_DEAD);
	//	}
	//}
	//for (auto e : mEnemyWave3)
	//{
	//	if (e != nullptr)
	//	{
	//		e->SetState(GameObject::STATE_DEAD);
	//	}
	//}
}

void EnemyManager::CreateEnemys()
{
	// 敵を先に生成しておく
	for (int i = 0; i < mMaxWeakEnemy; i++)
	{
		mWeakEnemys.emplace_back(new WeakEnemy(mTarget, Vector3(0, 0, -1000)));
		mWeakEnemys[i]->SetState(GameObject::STATE_PAUSED);
	}
	for (int i = 0; i < mMaxStrongEnemy; i++)
	{
		mStrongEnemys.emplace_back(new StrongEnemy(mTarget));
		mStrongEnemys[i]->SetPosition(Vector3(0,0,-1000));
		mStrongEnemys[i]->SetState(GameObject::STATE_PAUSED);
	}
	for (int i = 0; i < mMaxBossEnemy; i++)
	{
		mBossEnemys.emplace_back(new BossEnemy(mTarget));
		mBossEnemys[i]->SetPosition(Vector3(0, 0, -1000));
		mBossEnemys[i]->SetState(GameObject::STATE_PAUSED);
	}
}


//// 最初のウェーブ生成
//void EnemyManager::CreateFirstWave()
//{
//	for (int i = 0; i < 3; i++)
//	{
//		mEnemyWave0.emplace_back(new WeakEnemy(mTarget));
//		mEnemyWave0[i]->SetPosition(Vector3(1000.0f, -200.0f * (i - 1), 0)+mOffset);
//		//Vector3 weakEnemySpawnPos = Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0) + mOffset;
//		//mWeakEnemySpawner->SpawnEnemy(weakEnemySpawnPos);
//	}
//}
//
//void EnemyManager::CreateWave(int waveCount)
//{
//	switch (waveCount)
//	{
//	case 1:
//		// 敵ウェーブ1作成
//		for (int i = 0; i < 20; i++)
//		{
//			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
//			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, -200.0f * (i + 1), 0)+mOffset);
//			//mWeakEnemySpawner->SpawnEnemy(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0) + mOffset);
//		}
//
//		mEnemyWave1.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave1[5]->SetPosition(Vector3(1000.0f, 0.0f, 0) + mOffset);
//
//		for (int i = 6; i < 12; i++)
//		{
//			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
//			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, 200.0f * (i - 5), 0)+mOffset);
//		}
//		break;
//	case 2:
//		// 雑魚敵追加
//		for (int i = 0; i < 20; i++)
//		{
//			mEnemyWave2.emplace_back(new WeakEnemy(mTarget));
//			mEnemyWave2[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000),0)+mOffset);
//		}
//
//		// 強敵追加
//		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave2[20]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//		mEnemyWave2[21]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//
//		break;
//	case 3:
//		// 雑魚敵追加
//		for (int i = 0; i < 30; i++)
//		{
//			mEnemyWave3.emplace_back(new WeakEnemy(mTarget));
//			mEnemyWave3[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//		}
//
//		// 強敵の追加
//		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave3[30]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave3[31]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
//		mEnemyWave3[32]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
//
//		break;
//
//	case 4:
//		// ボス敵追加
//		mEnemyWave4.emplace_back(new BossEnemy(mTarget));
//		mEnemyWave4[0]->SetPosition(Vector3(1000.0f, 0.0f, 0)+mOffset);
//		break;
//	default: 
//		return;
//	}
//}

void EnemyManager::RemoveDeadEnemy()
{
}

void EnemyManager::Update(float deltaTime)
{
	mTimer += deltaTime;

	SpawnWeakEnemy();
	//// 1体でも生存状態の敵がいたら次のウェーブに行かない
	//if (GAMEINSTANCE.IsExistActorType(Tag::ENEMY))
	//{
	//	mIsNext = false;
	//}
	//else 
	//{ 
	//	mIsNext = true; 
	//}

	//// 次のウェーブに移動可能であれば
	//if (mIsNext)
	//{
	//	mWaveCount++;

	//	// 全てのウェーブが終わったらフラグをtrueに
	//	if (mWaveCount == mMaxEnemyWave)
	//	{
	//		mIsLastWave = true;
	//	}

	//	// ウェーブ数がリストサイズを超えたら
	//	if (mWaveCount >= mMaxEnemyWave)
	//	{
	//		return;
	//	}

	//	// 次ウェーブの敵を生成
	//	CreateWave(mWaveCount);
	//}
}

void EnemyManager::SpawnWeakEnemy()
{
	// 時間ごとに敵をスポーン
	const bool isSpawn = mTimer > mSpawnCoolTime;
	if (isSpawn)
	{
		mTimer = 0.0f;
		int count = 0;

		for (auto e : mWeakEnemys)
		{
			if (count >= 4)return;
			if (e->GetIsActive() == false)
			{
				Vector3 pos = Vector3(mEnemySpawnerEffect[count]->GetPosition().x, mEnemySpawnerEffect[count]->GetPosition().y, mEnemySpawnerEffect[count]->GetPosition().z - 20.0f);
				e->SetIsActive(true);
				e->SetState(GameObject::STATE_ACTIVE);
				e->SetPosition(pos);
				count++;
			}
		}
	}
}

void EnemyManager::SpawnStrongEnemy()
{
}

void EnemyManager::SpawnBossEnemy()
{
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

EnemyBase* EnemyManager::GetNearestEnemy(std::vector<EnemyBase*> enemys)
{
	for (auto e : enemys)
	{
		float distance = Math::Abs(mTarget->GetPosition().LengthSq() - e->GetPosition().LengthSq());
	}
	return enemys[0];
}

int EnemyManager::ActiveEnemyNum()
{
	int count = 0;

	// 敵配列を全て走査
	for (auto e : mWeakEnemys)
	{
		// 未使用中の敵がいれば
		const bool isActive = e->GetIsActive();
		if (!isActive)
		{
			count++;
			// 使用中フラグを立てる
			e->SetIsActive(true);
			e->SetState(GameObject::STATE_ACTIVE);
		}
	}

	return count;
}



