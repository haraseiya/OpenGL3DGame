#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"
#include "EnemySpawnerEffect.h"
#include <iostream>

const Vector3 EnemyManager::mEnemyStartPos = Vector3(5000, 5000, 5000);	// 敵初期位置

// 敵最大数の
const int EnemyManager::mMaxBossEnemy = 1;			// ボス敵最大数
const int EnemyManager::mMaxStrongEnemy = 8;		// 強敵最大数
const int EnemyManager::mMaxWeakEnemy = 30;			// 雑魚敵最大数
//const int EnemyManager::mMaxEnemyNum = 39;			// 敵全体の最大数

// 敵の発生源の初期位置
const Vector3 EnemyManager::mSpawnPoint1 = Vector3(2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint2 = Vector3(-2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint3 = Vector3(-2000, -2000, 750);
const Vector3 EnemyManager::mSpawnPoint4 = Vector3(2000, -2000, 750);

// スポーンする間隔
const float EnemyManager::mSpawnCoolTime = 10.0f;	

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
	mEnemySpawnerEffect[0] = new EnemySpawnerEffect(mSpawnPoint1);
	mEnemySpawnerEffect[1] = new EnemySpawnerEffect(mSpawnPoint2);
	mEnemySpawnerEffect[2] = new EnemySpawnerEffect(mSpawnPoint3);
	mEnemySpawnerEffect[3] = new EnemySpawnerEffect(mSpawnPoint4);

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

// 全てのオブジェクト削除処理
EnemyManager::~EnemyManager()
{
	// 雑魚敵
	for (auto e:mWeakEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}

	// 強敵
	for (auto e : mStrongEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}

	// ボス敵
	for (auto e : mBossEnemys)
	{
		e->SetState(GameObject::STATE_DEAD);
	}

	// スポーン用エフェクト
	for (auto e : mEnemySpawnerEffect)
	{
		delete e;
		e = nullptr;
	}
}

void EnemyManager::CreateEnemys()
{
	// 使用する敵をあらかじめ全て生成しておく
	for (int i = 0; i < mMaxWeakEnemy; i++)
	{
		mWeakEnemys.emplace_back(new WeakEnemy(mTarget, mEnemyStartPos));
		mWeakEnemys[i]->SetState(GameObject::STATE_PAUSED);
		mWeakEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	for (int i = 0; i < mMaxStrongEnemy; i++)
	{
		mStrongEnemys.emplace_back(new StrongEnemy(mTarget, mEnemyStartPos));
		mStrongEnemys[i]->SetState(GameObject::STATE_PAUSED);
		mStrongEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	for (int i = 0; i < mMaxBossEnemy; i++)
	{
		mBossEnemys.emplace_back(new BossEnemy(mTarget, mEnemyStartPos));
		mBossEnemys[i]->SetState(GameObject::STATE_PAUSED);
	}
}

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

// 雑魚敵の生成
void EnemyManager::SpawnWeakEnemy()
{
	// 雑魚敵全体を走査
	for (auto e : mWeakEnemys)
	{
		// 一度に4体まで生成
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

// 強敵の生成
void EnemyManager::SpawnStrongEnemy()
{
	// 強敵全体を走査
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
	// 敵が存在しなければ返す
	if (enemys.empty())return nullptr;

	std::vector<float> distance;
	for (auto e : enemys)
	{
		distance.emplace_back(Math::Abs(mTarget->GetPosition().LengthSq() - e->GetPosition().LengthSq()));
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



