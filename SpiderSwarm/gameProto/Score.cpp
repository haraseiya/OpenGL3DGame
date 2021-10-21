#include "Score.h"

Score::Score()
	: mSumScore(0)
{
	// UI�\��
}

Score::~Score()
{
}

void Score::AddScore(int score)
{
	mSumScore += score;
}

void Score::ScoreUpdate()
{
	// ���v�X�R�A���n�C�X�R�A�𒴂�����
	const bool isUpdateHightScore = mSumScore < mHighScore;
	if (isUpdateHightScore)
	{
		mHighScore = mSumScore;
	}

}
