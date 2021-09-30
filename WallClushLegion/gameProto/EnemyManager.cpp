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
	mEnemyWaveList.emplace_back(mEnemyWave);
	// �ŏ��̃E�F�[�u�𐶐�
	//CreateFirstWave();
	// ���w
	mEnemyWave.emplace_back(new StrongEnemy(mTarget));
	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mEnemyWave.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::CreateFirstWave()
{

}

void EnemyManager::CreateWave()
{
}

void EnemyManager::RemoveDeadEnemy()
{
}

void EnemyManager::Update(float deltaTime)
{
	mTime += deltaTime;

	// 1�̂ł�������Ԃ̓G�������玟�̃E�F�[�u�ɍs���Ȃ�
	if (GAMEINSTANCE.IsExistActorType(Tag::Enemy))
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
		printf("���̃E�F�[�u\n");
		mWaveCount++;

		// �S�ẴE�F�[�u���I�������t���O��true��
		if (mWaveCount == mEnemyWaveList.size() )
		{
			mIsLastWave = true;
		}

		// �E�F�[�u�������X�g�T�C�Y�𒴂�����
		if (mWaveCount >= mEnemyWaveList.size())
		{
			return;
		}

		// ���E�F�[�u�̓G���A�N�e�B�u��
		RemoveDeadEnemy();
		CreateWave();
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

