#pragma once

class Score
{
public:
	Score();
	~Score();

	void AddScore(int score);
	void ScoreUpdate();
	int GetScore() { return mSumScore; }

private:
	int mSumScore;
	int mHighScore;
};

