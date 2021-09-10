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
	// �G�̃E�F�[�u���X�g�ɃE�F�[�u��ǉ�
	mEnemyWaveList.push_back(mFirstWave);
	mEnemyWaveList.push_back(mSecondWave);
	mEnemyWaveList.push_back(mThirdWave);
	CreateFirstWave();
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateFirstWave()
{
	// ���w
	mFirstWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mFirstWave.push_back(new WeakEnemy(mTarget));
		mFirstWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

void EnemyManager::CreateWave()
{
	mSecondWave.push_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mSecondWave.push_back(new WeakEnemy(mTarget));
		mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}

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

	// 1�̂ł��A�N�e�B�u��Ԃ̓G�������玟�̃E�F�[�u�ɍs���Ȃ�
	for (auto enemy : mEnemyWaveList[mWaveCount])
	{
		// �G�����݂���ꍇ
		if (enemy->GetState() == GameObject::STATE_ACTIVE)
		{
			mIsNext = false;
		}
	}

	// ���̃E�F�[�u�Ɉړ��\�ł����
	if (mIsNext)
	{
		printf("���̃E�F�[�u\n");
		mWaveCount++;

		if (mWaveCount == mEnemyWaveList.size() - 1)
		{
			mIsLastWave = true;
		}

		if (mWaveCount >= mEnemyWaveList.size()) return;

		// ���E�F�[�u�̓G���A�N�e�B�u��
		for (auto enemy : mEnemyWaveList[mWaveCount])
		{
		}
	}
}

