#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"

const int EnemyManager::mMaxEnemyNum = 100;		// �G�S�̂̍ő吔
const int EnemyManager::mMaxEnemyWave = 5;		// �G�̍ő�E�F�[�u��
const int EnemyManager::mMaxBossEnemy = 1;		// �{�X�G�ő吔
const int EnemyManager::mMaxStrongEnemy = 3;	// ���G�ő吔
const int EnemyManager::mMaxWeakEnemy = 30;		// �G���G�ő吔

const float EnemyManager::mRandomRangeMinX = -2000;
const float EnemyManager::mRandomRangeMaxX = 2000;
const float EnemyManager::mRandomRangeMinY = -2000;
const float EnemyManager::mRandomRangeMaxY = 2000;

EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mTime(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
{
	mOffset = Vector3(0, 0, 500);

	//mEnemyWaveList.reserve(mEnemyWave0.size());
	//mEnemyWaveList.reserve(mEnemyWave1.size());
	//mEnemyWaveList.reserve(mEnemyWave2.size());
	//mEnemyWaveList.reserve(mEnemyWave3.size());
	//mEnemyWaveList.reserve(mEnemyWave4.size());

	// �T�C�Y�m��
	mEnemyWave0.reserve(3);
	mEnemyWave1.reserve(20);
	mEnemyWave2.reserve(30);
	mEnemyWave3.reserve(40);
	mEnemyWave4.reserve(1);
	
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
	for (int i = 0; i < 3; i++)
	{
		mEnemyWave0.emplace_back(new WeakEnemy(mTarget));
		mEnemyWave0[i]->SetPosition(Vector3(1000.0f, -200.0f * (i - 1), 0)+mOffset);
	}
}

void EnemyManager::CreateWave(int waveCount)
{
	switch (waveCount)
	{
	case 1:
		// �G�E�F�[�u1�쐬
		for (int i = 0; i < 5; i++)
		{
			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, -200.0f * (i + 1), 0)+mOffset);
		}

		mEnemyWave1.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave1[5]->SetPosition(Vector3(1000.0f, 0.0f, 0) + mOffset);

		for (int i = 6; i < 12; i++)
		{
			mEnemyWave1.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave1[i]->SetPosition(Vector3(1000.0f, 200.0f * (i - 5), 0)+mOffset);
		}
		break;
	case 2:
		// �G���G�ǉ�
		for (int i = 0; i < 20; i++)
		{
			mEnemyWave2.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave2[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000),0)+mOffset);
		}

		// ���G�ǉ�
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave2.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave2[20]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
		mEnemyWave2[21]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);

		break;
	case 3:
		// �G���G�ǉ�
		for (int i = 0; i < 30; i++)
		{
			mEnemyWave3.emplace_back(new WeakEnemy(mTarget));
			mEnemyWave3[i]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
		}

		// ���G�̒ǉ�
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[30]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[31]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);
		mEnemyWave3.emplace_back(new StrongEnemy(mTarget));
		mEnemyWave3[32]->SetPosition(Vector3(Math::GetRandom(-1000, 1000), Math::GetRandom(-1000, 1000), 0)+mOffset);

		break;

	case 4:
		// �{�X�G�ǉ�
		mEnemyWave4.emplace_back(new BossEnemy(mTarget));
		mEnemyWave4[0]->SetPosition(Vector3(1000.0f, 0.0f, 0)+mOffset);
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

EnemyBase* EnemyManager::GetNearEnemy(std::vector<EnemyBase*> enemys)
{
	for (auto e : enemys)
	{
		float distance = Math::Abs(mTarget->GetPosition().LengthSq() - e->GetPosition().LengthSq());
	}
	return enemys[0];
}


