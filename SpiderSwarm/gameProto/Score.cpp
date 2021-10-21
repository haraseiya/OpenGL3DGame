#include "Score.h"

Score::Score()
	: mSumScore(0)
{
	// UI表示
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
	// 合計スコアがハイスコアを超えたら
	const bool isUpdateHightScore = mSumScore < mHighScore;
	if (isUpdateHightScore)
	{
		mHighScore = mSumScore;
	}

}
