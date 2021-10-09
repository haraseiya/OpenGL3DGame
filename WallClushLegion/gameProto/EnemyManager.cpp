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
	// �G�E�F�[�u1�쐬
	for (int i = 0; i < 5; i++)
	{
		mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave1[i]->SetPosition(Vector3(1000.0f,-200.0f*(i+1),500.0f));
	}

	mEnemyWave1.emplace_back(new StrongEnemy(mTarget));

	mEnemyWave1[5]->SetPosition(Vector3(1000.0f, 0.0f, 500.0f));
	for (int i = 6; i < 12; i++)
	{
		mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave1[i]->SetPosition(Vector3(1000.0f, 200.0f * (i-5) , 500.0f));
	}
}

void EnemyManager::CreateWave(int waveCount)
{
	switch (waveCount)
	{
	case 1:
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		for (int i = 0; i < 20; i++)
		{
			mEnemyWave2.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave2[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 500));
		}
		break;
	case 2:
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		for (int i = 0; i < 30; i++)
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


