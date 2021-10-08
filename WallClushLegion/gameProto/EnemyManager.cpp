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
	// �ŏ��̃E�F�[�u�𐶐�
	CreateFirstWave();
}

EnemyManager::~EnemyManager()
{
	// �c�����G�l�~�[�̍폜
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
	// �G�E�F�[�u1�쐬
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

	// 1�̂ł�������Ԃ̓G�������玟�̃E�F�[�u�ɍs���Ȃ�
	if (GAMEINSTANCE.IsExistActorType(Tag::ENEMY))
	{
		mIsNext = false;
	}
	else 
	{ 
		mIsNext = true; 
	}

	// ���̃E�F�[�u�Ɉړ��\�ł����
	if (mIsNext)
	{
		mWaveCount++;

		// �S�ẴE�F�[�u���I�������t���O��true��
		if (mWaveCount == mMaxEnemyWave)
		{
			mIsLastWave = true;
		}

		// �E�F�[�u�������X�g�T�C�Y�𒴂�����
		if (mWaveCount >= mMaxEnemyWave)
		{
			return;
		}

		// ���E�F�[�u�̓G�𐶐�
		CreateWave(mWaveCount);
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}


