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

	mEnemyWaveList.reserve(mEnemyWaveList.size());

	// �ŏ��̃E�F�[�u�𐶐�
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
	// ���w
	mSecondWave.push_back(new StrongEnemy(mTarget));

	for (int i = 0; i < mMaxEnemyNum; i++)
	{
		mSecondWave.push_back(new WeakEnemy(mTarget));
		mSecondWave[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
	}

	// ��O�w
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
		if (mWaveCount == mEnemyWaveList.size() - 1)
		{
			mIsLastWave = true;
		}

		// �E�F�[�u�������X�g�T�C�Y�𒴂�����
		if (mWaveCount >= mEnemyWaveList.size()) return;

		// ���̃E�F�[�u���X�g��`��
		for (auto wave : mEnemyWaveList[mWaveCount])
		{

		}
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

