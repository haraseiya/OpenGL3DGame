#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"

const int EnemyManager::mMaxEnemyNum = 100;		// 敵全体の最大数
const int EnemyManager::mMaxEnemyWave = 4;		// 敵の最大ウェーブ数
const int EnemyManager::mMaxBossEnemy = 1;		// ボス敵最大数
const int EnemyManager::mMaxStrongEnemy = 3;	// 強敵最大数
const int EnemyManager::mMaxWeakEnemy = 30;		// 雑魚敵最大数

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

void EnemyManager::CreateFirstWave()
{
	for (int i = 0; i < 3; i++)
	{
		mEnemyWave0.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave0[i]->SetPosition(Vector3(1000.0f, -200.0f * (i + 1), 500.0f));
	}
}

void EnemyManager::CreateWave(int waveCount)
{
	switch (waveCount)
	{
	case 1:
		// 敵ウェーブ1作成
		for (int i = 0; i < 5; i++)
		{
			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, -200.0f * (i + 1), 500.0f));
		}

		mEnemyWave1.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave1[5]->SetPosition(Vector3(1000.0f, 0.0f, 500.0f));

		for (int i = 6; i < 12; i++)
		{
			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, 200.0f * (i - 5), 500.0f));
		}
		break;
	case 2:
		// 雑魚敵追加
		for (int i = 0; i < 20; i++)
		{
			mEnemyWave2.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave2[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		}

		// 強敵追加
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave2[20]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		mEnemyWave2[21]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));

		break;
	case 3:
		// 雑魚敵追加
		for (int i = 0; i < 30; i++)
		{
			mEnemyWave3.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave3[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		}

		// 強敵の追加
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[30]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[31]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[32]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));

		break;

	case 4:
		// ボス敵追加
		mEnemyWave4.emplace_back(new BossEnemy(mTarget));
		mEnemyWave4[0]->SetPosition(Vector3(1000.0f, 0.0f, 500.0f));
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

EnemyBase* EnemyManager::GetNearEnemy(std::vector<EnemyBase*> enemys)
{
	for (auto e : enemys)
	{
		float distance = Math::Abs(mTarget->GetPosition().LengthSq() - e->GetPosition().LengthSq());
	}
	return enemys[0];
}


