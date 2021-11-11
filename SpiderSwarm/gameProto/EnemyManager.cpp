#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"
#include "EnemySpawnerEffect.h"
#include <iostream>

//EnemyManager* EnemyManager::mInstance = nullptr;// シングルトン生成
const Vector3 EnemyManager::mEnemyStartPos = Vector3(5000, 5000, 5000);	// 敵初期位置

const int EnemyManager::mMaxEnemyNum = 30;			// 敵全体の最大数
const int EnemyManager::mMaxBossEnemy = 1;			// ボス敵最大数
const int EnemyManager::mMaxStrongEnemy = 8;		// 強敵最大数
const int EnemyManager::mMaxWeakEnemy = 30;			// 雑魚敵最大数
const float EnemyManager::mSpawnCoolTime = 10.0f;	// スポーンする間隔

const float EnemyManager::mRandomRangeMinX = -2000;
const float EnemyManager::mRandomRangeMaxX = 2000;
const float EnemyManager::mRandomRangeMinY = -2000;
const float EnemyManager::mRandomRangeMaxY = 2000;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mWeakEnemyTimer(0.0f)
	, mStrongEnemyTimer(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
	, mWeakEnemyCount(0)
	, mEnemyCount(0)
{
	mOffset = Vector3(0, 0, 750);

	// 敵のスポーン位置を定義
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
	for (auto e:mWeakEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
	for (auto e : mStrongEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
	for (auto e : mBossEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}

	for (auto e : mEnemySpawnerEffect)
	{
		e->SetState(GameObject::STATE_DEAD);
	}
}

void EnemyManager::CreateEnemys()
{
	// 敵を先に生成しておく
	for (int i = 0; i < mMaxWeakEnemy; i++)
	{
		mWeakEnemys.emplace_back(new WeakEnemy(mTarget, mEnemyStartPos));
		mWeakEnemys[i]->SetState(GameObject::STATE_PAUSED);
		mWeakEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	for (int i = 0; i < mMaxStrongEnemy; i++)
	{
		mStrongEnemys.emplace_back(new StrongEnemy(mTarget,mEnemyStartPos));
		mStrongEnemys[i]->SetState(GameObject::STATE_PAUSED);
		mStrongEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	for (int i = 0; i < mMaxBossEnemy; i++)
	{
		mBossEnemys.emplace_back(new BossEnemy(mTarget,mEnemyStartPos));
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


void EnemyManager::RemoveDeadEnemy()
{
}

void EnemyManager::Update(float deltaTime)
{
	mWeakEnemyTimer += deltaTime;
	mStrongEnemyTimer += deltaTime;

	// 時間ごとに敵をスポーン
	const float spawnWeakEnemyCoolTime = 10.0f;
	const bool isWeakEnemySpawn = mWeakEnemyTimer > spawnWeakEnemyCoolTime;
	if (isWeakEnemySpawn)
	{
		mWeakEnemyTimer = 0.0f;
		SpawnWeakEnemy();
	}

	const float spawnStrongEnemyCoolTime = 60.0f;
	const bool isStrongEnemySpawn = mStrongEnemyTimer > spawnStrongEnemyCoolTime;
	if (isStrongEnemySpawn)
	{
		mStrongEnemyTimer = 0.0f;
		SpawnStrongEnemy();
	}


}

void EnemyManager::SpawnWeakEnemy()
{
	for (auto e : mWeakEnemys)
	{
		if (mWeakEnemyCount > 3) 
		{
			mWeakEnemyCount = 0;
			return; 
		}

		// 使用されていない敵がいれば
		if (e->GetIsActive() == false)
		{
			Vector3 pos = Vector3(mEnemySpawnerEffect[mWeakEnemyCount]->GetPosition());
			e->SetIsActive(true);
			e->SetState(GameObject::STATE_ACTIVE);
			e->SetPosition(pos);
			mWeakEnemyCount++;
		}
	}
}

void EnemyManager::SpawnStrongEnemy()
{
	for (auto e : mStrongEnemys)
	{
		if (mStorngEnemyCount > 3)
		{
			mStorngEnemyCount = 0;
			return;
		}

		if (e->GetIsActive() == false)
		{
			Vector3 pos = Vector3(mEnemySpawnerEffect[mStorngEnemyCount]->GetPosition());
			e->SetIsActive(true);
			e->SetState(GameObject::STATE_ACTIVE);
			e->SetPosition(pos);
			mStorngEnemyCount++;
		}
	}
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

int EnemyManager::GetActiveEnemyNum()
{
	int count = 0;

	// 雑魚敵配列を全て走査
	for (auto e : mWeakEnemys)
	{
		// アクティブの敵がいれば
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	// 強敵配列を全て走査
	for (auto e : mStrongEnemys)
	{
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}
	return count;
}



