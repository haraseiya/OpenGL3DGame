#include "ScoreManager.h"

ScoreManager* ScoreManager::mScoreManager = nullptr;

ScoreManager::ScoreManager()
	: mSumScore(0)
{
}

void ScoreManager::Initialize()
{
	mSumScore = 0;
}

void ScoreManager::AddScore(int score)
{
	mSumScore += score;
}

//void ScoreManager::ScoreUpdate()
//{
//	// ���v�X�R�A���n�C�X�R�A�𒴂�����
//	const bool isUpdateHightScore = mSumScore < mHighScore;
//	if (isUpdateHightScore)
//	{
//		mHighScore = mSumScore;
//	}
//}

void ScoreManager::CreateInstance()
{
	if (mScoreManager == nullptr)
	{
		mScoreManager = new ScoreManager();
	}
}

void ScoreManager::RemoveInstance()
{
	if (mScoreManager != nullptr)
	{
		delete mScoreManager;
		mScoreManager = nullptr;
	}
}

